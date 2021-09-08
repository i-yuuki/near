#pragma once

#include "vertex.h"
#include "buffer.h"
#include "texture.h"

namespace Near{

struct FBXMesh{
  std::vector<Vertex3D> vertices;
  int indexCount;
  int* indices;
  int polygonCount;
  int material;
  Math::Matrix transform;
  Math::Matrix customTransform;
  VertexBuffer buffer;
  bool visible;
};

// 仮
struct Material{
  Math::Color ambient;
  Math::Color diffuse;
  Math::Color emissive;
};

struct FBXMaterial{
  Material material;
  Texture texture;
};

// FBXModel::load()に使うオプションをまとめたオブジェクトです。
// 関数をつなげて分かりやすくオプションを指定できます。
// 使用例:
// model->load("hoge.fbx", FBXLoadOptions().correctGamma(false));
class FBXLoadOptions{
public:
  bool _flipV = true;
  bool _correctGamma = true;
  bool _triangulate = false;
  bool _splitMeshesPerMaterial = false;
  // UVのVを反転 (1→0, 0.2→0.8) します。
  // デフォルト: true
  inline FBXLoadOptions& flipV(bool v){ _flipV = v; return *this; }
  // 全マテリアルの色 (ambient・diffuseなど) にsRGBガンマ補正 (γ=1/2.2くらい) をかけて明るくします。
  // デフォルト: true
  inline FBXLoadOptions& correctGamma(bool v){ _correctGamma = v; return *this; }
  // ポリゴンを三角形にします。三角形以外のポリゴンを含む可能性のあるモデルでぜひ。
  // 1秒でも読み込みを速くするため、事前に三角形化したモデルを使うことも検討してください。
  // デフォルト: false
  inline FBXLoadOptions& triangulate(bool v){ _triangulate = v; return *this; }
  // メッシュをマテリアルごとに分割します。
  // FBXModelはメッシュごとに最初のマテリアルを使うため、一つのメッシュが複数のマテリアルを
  // 持つモデルでこのオプションをオフにすると表示が乱れます。
  // 1秒でも読み込みを速くするため、事前にメッシュを分割したモデルを使うことも検討してください。
  // デフォルト: false
  inline FBXLoadOptions& splitMeshesPerMaterial(bool v){ _splitMeshesPerMaterial = v; return *this; }
};

class FBXModel{
public:
  FBXModel();
  ~FBXModel();
  void load(const char* file, FBXLoadOptions options = FBXLoadOptions());
  void setAnimation(int idx, double seconds);
  void draw(const Math::Matrix* transform = nullptr);
  inline double getAnimationTime() { return animationTime; }
  FBXMesh* findMeshByNodeName(const char* name);
protected:
  class Internal;
  std::unique_ptr<Internal> internal = std::make_unique<Internal>();
  std::vector<FBXMesh> meshes;
  std::vector<FBXMaterial> materials;
  int animationIdx;
  double animationTime;
  double animationStartTime;
  double animationDuration;
  static constexpr int VERTEX_SHADER_MATRICES = 60;
};

}
