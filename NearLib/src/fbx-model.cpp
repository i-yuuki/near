#include "pch.h"
#include "fbx-model.h"

#include <unordered_map>
#include <fbxsdk.h>

#include "texture.h"
#include "utils.h"

namespace Near{

class FBXModel::Internal{
public:
  fbxsdk::FbxManager* manager = nullptr;
  fbxsdk::FbxScene* scene = nullptr;
  fbxsdk::FbxTime fbxAnimationTime = 0;
  std::vector<fbxsdk::FbxNode*> meshNodes{};
  ~Internal(){
    safeDestroy(scene);
    safeDestroy(manager);
  }
};

template<class T>
static inline void fbxMatrixToDXMatrix(T* fbx, Math::Matrix* dx){
  for (int row = 0;row < 4;row ++){
    for (int col = 0;col < 4;col ++){
      dx->m[row][col] = static_cast<float>(fbx->Get(row, col));
    }
  }
}

FBXModel::FBXModel(){
  internal->manager = fbxsdk::FbxManager::Create();
  if(!internal->manager) throw std::exception("FbxManager::Create() failed");
}

FBXModel::~FBXModel(){
  // for(int i = 0;i < materials.getSize();i ++){
  //   textureRelease(&materials[i].texture, 1);
  // }
  for(auto& mesh : meshes){
    mesh.buffer.uninit();
  }
}

float decodeGamma(float v){
  // sRGBのガンマ補正: 2.2曲線で近似することもあるが厳密には低輝度で直線、ほかで2.4曲線とかなんとか
  // https://qiita.com/yoya/items/122b93970c190068c752#srgb-%E3%81%AE%E3%82%AC%E3%83%B3%E3%83%9E%E8%A3%9C%E6%AD%A3
  if(v <= 0.0031308f){
    return v * 12.92f;
  }else{
    return 1.055f * powf(v, 1 / 2.4f) - 0.055f;
  }
}

void FBXModel::load(const char* file, FBXLoadOptions options){
  // FBXシーンにインポート
  fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(internal->manager, file);
  if(!importer) throw std::exception("FbxImporter::Create() failed");
  
  internal->scene = fbxsdk::FbxScene::Create(internal->manager, file);
  if(!internal->scene){
    importer->Destroy();
    throw std::exception("FbxScene::Create() failed");
  }
  
  if(!importer->Initialize(file)){
    importer->Destroy();
    throw std::exception("FbxImporter initialize failed");
  }
  if(!importer->Import(internal->scene)){
    importer->Destroy();
    throw std::exception("FbxImporter import failed");
  }
  importer->Destroy();
  
  // 三角ポリゴンだけにする
  fbxsdk::FbxGeometryConverter converter(internal->manager);
  // legacy = true: 穴があいたジオメトリに対応しない
  if(options._triangulate && !converter.Triangulate(internal->scene, true, false)) throw std::exception("Scene triangulation failed");
  
  if(options._splitMeshesPerMaterial && !converter.SplitMeshesPerMaterial(internal->scene, true)) throw std::exception("Mesh split per material failed");
  
  // シーンの単位をmにする (?)
  fbxsdk::FbxSystemUnit unit = internal->scene->GetGlobalSettings().GetSystemUnit();
  fbxsdk::FbxSystemUnit::m.ConvertScene(internal->scene);
  
  int materialCount = internal->scene->GetMaterialCount();
  materials.resize(materialCount);
  // マテリアルをインデックスで管理したいので変換用マップ
  std::unordered_map<const char*, int> materialNames = std::unordered_map<const char*, int>();
  for(int i = 0;i < materialCount;i ++){
    FBXMaterial* material = &materials[i];
    material->material = {};
    material->material.ambient = {0.2f, 0.2f, 0.2f, 1.0f};
    material->material.diffuse = {0.8f, 0.8f, 0.8f, 1.0f};
    fbxsdk::FbxSurfaceMaterial* fbxMaterial = internal->scene->GetMaterial(i);
    materialNames.insert(std::pair<const char*, int>(fbxMaterial->GetName(), i));
    printf_s(u8"    #%3d: %s\n", i + 1, fbxMaterial->GetName());
    
    fbxsdk::FbxProperty fbxAmbientProp = fbxMaterial->FindProperty(fbxsdk::FbxSurfaceMaterial::sAmbient);
    fbxsdk::FbxProperty fbxDiffuseProp = fbxMaterial->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse);
    fbxsdk::FbxProperty fbxEmissiveProp = fbxMaterial->FindProperty(fbxsdk::FbxSurfaceMaterial::sEmissive);
    fbxsdk::FbxProperty fbxTransparencyProp = fbxMaterial->FindProperty(fbxsdk::FbxSurfaceMaterial::sTransparencyFactor);
    if(fbxAmbientProp.IsValid()){
      fbxsdk::FbxDouble3 ambient = fbxAmbientProp.Get<fbxsdk::FbxDouble3>();
      if(options._correctGamma){
        material->material.ambient.x = decodeGamma(static_cast<float>(ambient[0]));
        material->material.ambient.y = decodeGamma(static_cast<float>(ambient[1]));
        material->material.ambient.z = decodeGamma(static_cast<float>(ambient[2]));
      }else{
        material->material.ambient.x = static_cast<float>(ambient[0]);
        material->material.ambient.y = static_cast<float>(ambient[1]);
        material->material.ambient.z = static_cast<float>(ambient[2]);
      }
      printf_s(u8"    ├ Ambient: %.3f, %.3f, %.3f\n", material->material.ambient.x, material->material.ambient.y, material->material.ambient.z);
    }else{
      printf_s(u8"    ├ Ambient なし\n");
    }
    if(fbxDiffuseProp.IsValid()){
      fbxsdk::FbxDouble4 diffuse = fbxDiffuseProp.Get<fbxsdk::FbxDouble4>();
      if(options._correctGamma){
        material->material.diffuse.x = decodeGamma(static_cast<float>(diffuse[0]));
        material->material.diffuse.y = decodeGamma(static_cast<float>(diffuse[1]));
        material->material.diffuse.z = decodeGamma(static_cast<float>(diffuse[2]));
      }else{
        material->material.diffuse.x = static_cast<float>(diffuse[0]);
        material->material.diffuse.y = static_cast<float>(diffuse[1]);
        material->material.diffuse.z = static_cast<float>(diffuse[2]);
      }
      material->material.ambient.x = material->material.diffuse.x * 0.3f;
      material->material.ambient.y = material->material.diffuse.y * 0.3f;
      material->material.ambient.z = material->material.diffuse.z * 0.3f;
      // material->material.diffuse.a = 0.2f;
      printf_s(u8"    ├ Diffuse: %.3f, %.3f, %.3f, %.3f\n", material->material.diffuse.x, material->material.diffuse.y, material->material.diffuse.z);
      int textureCount = fbxDiffuseProp.GetSrcObjectCount<fbxsdk::FbxFileTexture>();
      printf_s(u8"    ├ %d テクスチャ\n", textureCount);
      if(textureCount > 0){
        fbxsdk::FbxFileTexture* texture = fbxDiffuseProp.GetSrcObject<fbxsdk::FbxFileTexture>(0);
        if(!texture) continue;
        // モデルフォルダのテクスチャファイルパスを作る
        // (assets/models/hoge.fbx + ~/Desktop/sushi.png = assets/models/sushi.png)
        fbxsdk::FbxString texturePath = fbxsdk::FbxPathUtils::GetFolderName(file) + "/" + fbxsdk::FbxPathUtils::ChangeExtension(fbxsdk::FbxPathUtils::GetFileName(texture->GetFileName()), ".png");
        // fbxsdk::FbxString texturePath = fbxsdk::FbxPathUtils::GetFolderName(file) + "/" + fbxsdk::FbxPathUtils::GetRelativeFilePath(file, texture->GetFileName());
        printf_s(u8"    │ ├ #1: %s\n", texturePath.Buffer());
        material->texture.load(texturePath.Buffer());
      }
    }else{
      printf_s(u8"    ├ Diffuse なし\n");
    }
    if(fbxEmissiveProp.IsValid()){
      fbxsdk::FbxDouble3 emissive = fbxEmissiveProp.Get<fbxsdk::FbxDouble3>();
      if(options._correctGamma){
        material->material.emissive.x = decodeGamma(static_cast<float>(emissive[0]));
        material->material.emissive.y = decodeGamma(static_cast<float>(emissive[1]));
        material->material.emissive.z = decodeGamma(static_cast<float>(emissive[2]));
      }else{
        material->material.emissive.x = static_cast<float>(emissive[0]);
        material->material.emissive.y = static_cast<float>(emissive[1]);
        material->material.emissive.z = static_cast<float>(emissive[2]);
      }
      printf_s(u8"    ├ Emissive: %.3f, %.3f, %.3f\n", material->material.emissive.x, material->material.emissive.y, material->material.emissive.z);
    }else{
      printf_s(u8"    ├ Emissive なし\n");
    }
    if(fbxTransparencyProp.IsValid()){
      material->material.diffuse.z = 1 - static_cast<float>(fbxTransparencyProp.Get<fbxsdk::FbxDouble>());
      printf_s(u8"    ├ Diffuse Alpha: %.3f\n", material->material.diffuse.z);
    }else{
      printf_s(u8"    ├ Diffuse Alpha なし\n");
    }
  }
  
  int meshCount = internal->scene->GetSrcObjectCount<fbxsdk::FbxMesh>();
  printf_s(u8"  %d メッシュ (諸々変換後)\n", meshCount);
  meshes.resize(meshCount);
  internal->meshNodes.resize(meshCount);
  
  for(int meshIdx = 0;meshIdx < meshCount;meshIdx ++){
    fbxsdk::FbxMesh* fbxMesh = internal->scene->GetSrcObject<fbxsdk::FbxMesh>(meshIdx);
    int vertexCount = fbxMesh->GetControlPointsCount();
    int indexCount = fbxMesh->GetPolygonVertexCount();
    int* indices = fbxMesh->GetPolygonVertices();
    FBXMesh* mesh = &meshes[meshIdx];
    internal->meshNodes[meshIdx] = fbxMesh->GetNode();
    fbxsdk::FbxNode* fbxNode = internal->meshNodes[meshIdx];
    mesh->indexCount = indexCount;
    mesh->indices = indices;
    mesh->polygonCount = fbxMesh->GetPolygonCount();
    mesh->customTransform = Math::Matrix::Identity;
    mesh->visible = true;
    printf_s(u8"  ├ #%3d: \n", meshIdx + 1);
    // printf_s(u8"  │ ├ %-30s * %5d 頂点 * %5d インデックス\n", fbxMesh->GetName(), vertexCount, indexCount);
    // printf_s(u8"  │ ├ ノード %-23s\n", fbxNode->GetName());
    
    fbxsdk::FbxVector4* fbxVertices = fbxMesh->GetControlPoints();
    fbxsdk::FbxAMatrix transformGeometry = fbxsdk::FbxAMatrix(
      fbxNode->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot),
      fbxNode->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot),
      fbxNode->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot)
    );
    fbxsdk::FbxAMatrix transformLocal = fbxNode->EvaluateLocalTransform();
    fbxsdk::FbxAMatrix transformParent = fbxsdk::FbxAMatrix();
    transformParent.SetIdentity();
    for(fbxsdk::FbxNode* parent = fbxNode->GetParent();parent;parent = parent->GetParent()){
      transformParent = parent->EvaluateLocalTransform() * transformParent;
    }
    transformGeometry = transformParent * transformLocal * transformGeometry;
    fbxMatrixToDXMatrix(&transformGeometry, &mesh->transform);
    Vertex3D* vertices = new Vertex3D[vertexCount];
    for(int vertexIdx = 0;vertexIdx < vertexCount;vertexIdx ++){
      Vertex3D* v = &vertices[vertexIdx];
      v->position.x = static_cast<float>(fbxVertices[vertexIdx][0]);
      v->position.y = static_cast<float>(fbxVertices[vertexIdx][1]);
      v->position.z = static_cast<float>(fbxVertices[vertexIdx][2]);
      v->color = Math::Color(1, 1, 1, 1);
    }
    
    if(fbxMesh->GetElementMaterialCount() > 0){
      // マテリアルごとにメッシュを分割してあるので、最初のマテリアルがこのメッシュ全体のマテリアル
      const char* materialName = fbxNode->GetSrcObject<fbxsdk::FbxSurfaceMaterial>(fbxMesh->GetElementMaterial(0)->GetIndexArray().GetAt(0))->GetName();
      auto it = materialNames.find(materialName);
      mesh->material = it == materialNames.end() ? -1 : it->second;
      printf_s(u8"  │ │ ├ %s (#%d)\n", materialName, mesh->material + 1);
    }else{
      mesh->material = -1;
    }
    
    mesh->vertices.resize(indexCount);
    for(int indexIdx = 0;indexIdx < indexCount;indexIdx ++){
      mesh->vertices[indexIdx] = vertices[indices[indexIdx]];
    }
    safeDeleteArray(vertices);
    
    fbxsdk::FbxStringList uvSetNames;
    fbxMesh->GetUVSetNames(uvSetNames);
    // printf_s(u8"  │ ├ %d UVセット\n", uvSetNames.GetCount());
    if(uvSetNames.GetCount() > 0){
      fbxsdk::FbxString uvSetName = uvSetNames.GetStringAt(0);
      auto it = materialNames.find(uvSetName);
      fbxsdk::FbxArray<fbxsdk::FbxVector2> uvs;
      fbxMesh->GetPolygonVertexUVs(uvSetName, uvs);
      // printf_s(u8"  │ │ ├ #1: %s * %d UV\n", uvSetName, uvs.Size());
      for(int i = 0, uvCount = uvs.Size();i < uvCount;i ++){
        Vertex3D* v = &mesh->vertices[i];
        v->uv.x = static_cast<float>(uvs[i][0]);
        v->uv.y = options._flipV ? 1 - static_cast<float>(uvs[i][1]) : static_cast<float>(uvs[i][1]);
      }
    }
    
    fbxsdk::FbxArray<fbxsdk::FbxVector4> normals;
    fbxMesh->GetPolygonVertexNormals(normals);
    int normalCount = normals.Size();
    // printf_s(u8"  │ ├ %d 法線\n", normalCount);
    if(normalCount != indexCount){
      printf_s(u8"メッシュ #%d: %d インデックス vs %d 法線\n", meshIdx + 1, indexCount, normalCount);
    }
    for(int normalIdx = 0;normalIdx < normalCount;normalIdx ++){
      Vertex3D* v = &mesh->vertices[normalIdx];
      v->normal.x = static_cast<float>(normals[normalIdx][0]);
      v->normal.y = static_cast<float>(normals[normalIdx][1]);
      v->normal.z = static_cast<float>(normals[normalIdx][2]);
    }
    
    mesh->buffer.init(false, mesh->vertices.size() * sizeof(Vertex3D), mesh->vertices.data());
  }
  
  int animationStackCount = internal->scene->GetSrcObjectCount<fbxsdk::FbxAnimStack>();
  for(int i = 0;i < animationStackCount;i ++){
    fbxsdk::FbxAnimStack* stack = internal->scene->GetSrcObject<fbxsdk::FbxAnimStack>(i);
  }

  animationIdx = -1;
  animationTime = 0;
  animationDuration = 1;
  animationStartTime = 0;
  internal->fbxAnimationTime.SetTime(0, 0, 0);
  
  for(int i = 0;i < meshes.size();i ++){
    FBXMesh* mesh = &meshes[i];
    fbxsdk::FbxNode* fbxNode = internal->meshNodes[i];
    fbxsdk::FbxMatrix global = fbxNode->EvaluateGlobalTransform();
    fbxMatrixToDXMatrix(&global, &mesh->transform);
  }
}

FBXMesh* FBXModel::findMeshByNodeName(const char* name){
  for(int i = 0;i < meshes.size();i ++){
    if(strcmp(internal->meshNodes[i]->GetNameOnly(), name) == 0) return &meshes[i];
  }
  return NULL;
}

void FBXModel::setAnimation(int idx, double seconds) {
  if (animationIdx == idx && animationTime == seconds) return; // 再計算の必要なし
  if (animationIdx != idx) {
    if (idx < 0) {
      internal->scene->SetCurrentAnimationStack(NULL);
      animationStartTime = 0;
      animationDuration = 1;
    } else {
      fbxsdk::FbxAnimStack* stack = internal->scene->GetSrcObject<fbxsdk::FbxAnimStack>(idx);
      internal->scene->SetCurrentAnimationStack(stack);
      if (stack) {
        animationStartTime = stack->GetLocalTimeSpan().GetStart().GetSecondDouble();
        animationDuration = stack->GetLocalTimeSpan().GetDuration().GetSecondDouble();
      } else {
        animationStartTime = 0;
        animationDuration = 1;
      }
    }
    animationIdx = idx;
  }
  animationTime = fmod(seconds, animationDuration);
  if (animationIdx < 0) {
    internal->fbxAnimationTime = FBXSDK_TIME_INFINITE;
  } else {
    internal->fbxAnimationTime.SetSecondDouble(animationTime + animationStartTime);
  }
  Math::Matrix matrix;
  for (int i = 0; i < meshes.size(); i++) {
    FBXMesh* mesh = &meshes[i];
    fbxsdk::FbxNode* fbxNode = internal->meshNodes[i];
    fbxsdk::FbxMatrix global = fbxNode->EvaluateGlobalTransform(internal->fbxAnimationTime);
    fbxMatrixToDXMatrix(&global, &mesh->transform);
  }
}

void FBXModel::draw(const Math::Matrix* transform){
  auto* r = renderer();
  Math::Matrix world;
  
  for(int i = 0, l = meshes.size();i < l;i ++){
    FBXMesh* mesh = &meshes[i];
    if(!mesh->visible) continue;
    world = (transform ? mesh->transform * *transform : mesh->transform) * mesh->customTransform;
    r->pushWorldTransform();
    r->applyWorldTransform(world);
    if(mesh->material >= 0){
      FBXMaterial* material = &materials[mesh->material];
      r->setTexture(&material->texture);
    }else{
      r->setTexture(nullptr);
    }
    mesh->buffer.draw(sizeof(Vertex3D), 0, mesh->indexCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    r->popWorldTransform();
  }
}

}
