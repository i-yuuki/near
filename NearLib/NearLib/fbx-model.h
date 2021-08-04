#pragma once

#include "vertex.h"
#include "buffer.h"

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
  // UVのVを反転 (1→0, 0.2→0.8) します。
  // デフォルト: true
  inline FBXLoadOptions& flipV(bool v){ _flipV = v; return *this; }
  // 全マテリアルの色 (ambient・diffuseなど) にsRGBガンマ補正 (γ=1/2.2くらい) をかけて明るくします。
  // デフォルト: true
  inline FBXLoadOptions& correctGamma(bool v){ _correctGamma = v; return *this; }
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
