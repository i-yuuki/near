#include "level-object.h"

#include <NearLib/scene.h>
#include <NearLib/utils.h>

static int g_faceCornerIndices[6][4] = {
  {1, 5, 2, 6},
  {0, 1, 3, 2},
  {4, 0, 7, 3},
  {5, 4, 6, 7},
  {3, 2, 7, 6},
  {4, 5, 0, 1},
};
static Near::Math::Vector3 g_faceNormals[6] = {
  Near::Math::Vector3( 1,  0,  0), // 東
  Near::Math::Vector3( 0,  0,  1), // 南
  Near::Math::Vector3(-1,  0,  0), // 西
  Near::Math::Vector3( 0,  0, -1), // 北
  Near::Math::Vector3( 0,  1,  0), // 上
  Near::Math::Vector3( 0, -1,  0), // 下
};
static Near::Math::Color g_tmpFaceColors[5] = {
  Near::Math::Color(1, 1, 1, 1),
  Near::Math::Color(1.000f, 0.231f, 0.188f, 1),
  Near::Math::Color(1.000f, 0.800f, 0.000f, 1),
  Near::Math::Color(0.157f, 0.804f, 0.255f, 1),
  Near::Math::Color(0.000f, 0.478f, 1.000f, 1),
};

LevelObject::LevelObject(std::shared_ptr<Level> level) : Near::GameObject(), level(level){
}

void LevelObject::init(Near::Layer* layer){
  Near::GameObject::init(layer);

  std::vector<Near::Vertex3D> vertices;
  std::vector<uint32_t> indices;
  std::vector<uint32_t> indicesWireframe;
  uint32_t index = 0;
  DirectX::XMFLOAT3 blockCorners[8];
  for(auto& block : level->getBlocks()){
    DirectX::BoundingOrientedBox(block.position, block.size / 2, Near::Math::Quaternion::CreateFromYawPitchRoll(block.rotation.y, block.rotation.x, block.rotation.z)).GetCorners(blockCorners);
    for(int face = 0;face < 6;face ++){
      if(block.materials[face] == 0) continue;
      vertices.push_back({blockCorners[g_faceCornerIndices[face][0]], g_faceNormals[face], g_tmpFaceColors[(block.materials[face] - 1) % 5], Near::Math::Vector2(0, 0)});
      vertices.push_back({blockCorners[g_faceCornerIndices[face][1]], g_faceNormals[face], g_tmpFaceColors[(block.materials[face] - 1) % 5], Near::Math::Vector2(1, 0)});
      vertices.push_back({blockCorners[g_faceCornerIndices[face][2]], g_faceNormals[face], g_tmpFaceColors[(block.materials[face] - 1) % 5], Near::Math::Vector2(0, 1)});
      vertices.push_back({blockCorners[g_faceCornerIndices[face][3]], g_faceNormals[face], g_tmpFaceColors[(block.materials[face] - 1) % 5], Near::Math::Vector2(1, 1)});
      indices.push_back(index);
      indices.push_back(index + 1);
      indices.push_back(index + 2);
      indices.push_back(index + 2);
      indices.push_back(index + 1);
      indices.push_back(index + 3);
      indicesWireframe.push_back(index);
      indicesWireframe.push_back(index + 1);
      indicesWireframe.push_back(index + 1);
      indicesWireframe.push_back(index + 3);
      indicesWireframe.push_back(index + 3);
      indicesWireframe.push_back(index + 2);
      indicesWireframe.push_back(index + 2);
      indicesWireframe.push_back(index);
      index += 4;
    }
  }
  vertexBuffer.init(false, vertices.size(), vertices.data());
  indexBuffer.init(false, indices.size(), indices.data());
  indexBufferWireframe.init(false, indicesWireframe.size(), indicesWireframe.data());
  
  vertexShader = Near::Assets::vertexShaders()->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = Near::Assets::pixelShaders()->getOrLoad("assets/shaders/ps-level.hlsl");
  pixelShaderWireframe = Near::Assets::pixelShaders()->getOrLoad("assets/shaders/ps-wireframe.hlsl");
}

void LevelObject::draw(){
  auto* r = Near::renderer();
  r->setVertexShader(vertexShader.get());
  r->setPixelShader(pixelShader.get());
  vertexBuffer.draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indexBuffer);
}

void LevelObject::uninit(){
  vertexBuffer.uninit();
  indexBuffer.uninit();
  indexBufferWireframe.uninit();
}

void LevelObject::addColliders(std::function<void(const Near::Collision::BoundingBox3D&)> out){
  if(!level) return;
  for(auto& block : level->getBlocks()){
    out(Near::Collision::BoundingBox3D(block.position, block.size / 2));
  }
}

Level* LevelObject::getLevel(){
  return level.get();
}
