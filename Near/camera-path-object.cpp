#include "camera-path-object.h"

#include <random>

CameraPathObject::CameraPathObject(CameraPath* path) : Near::GameObject(), path(path){
}

void CameraPathObject::init(Near::Layer* layer){
  generatePath();
  vertexShader = Near::Assets::vertexShaders()->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = Near::Assets::pixelShaders()->getOrLoad("assets/shaders/ps-notex.hlsl");
}

void CameraPathObject::draw(){
  if(!visible) return;
  auto* r = Near::renderer();
  r->setVertexShader(vertexShader.get());
  r->setPixelShader(pixelShader.get());
  vertexBuffer.draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, indexBuffer);
}

void CameraPathObject::uninit(){
  vertexBuffer.uninit();
  indexBuffer.uninit();
}

void CameraPathObject::generatePath(){
  auto& points = path->getPoints();
  std::vector<Near::Vertex3D> vertices;
  std::vector<uint32_t> indices;
  for(size_t i = 1;i < points.size();i ++){
    GenerateVertices(points[i - 1], points[i], vertices, indices);
  }
  // バッファ作り直し
  // 新しいパスで頂点数が増えてるかもなのでsetはやめとくよ
  vertexBuffer.uninit();
  indexBuffer.uninit();
  vertexBuffer.init(false, static_cast<unsigned int>(vertices.size()), vertices.data());
  indexBuffer.init(false, static_cast<unsigned int>(indices.size()), indices.data());
}

bool CameraPathObject::isVisible(){
  return visible;
}

void CameraPathObject::setVisible(bool visible){
  this->visible = visible;
}

void CameraPathObject::GenerateVertices(const CameraPath::BezierPoint& from, const CameraPath::BezierPoint& to, std::vector<Near::Vertex3D>& vertices, std::vector<uint32_t>& indices){
  constexpr int verticesPerSegment = 10;
  std::random_device seed;
  std::mt19937 rand(seed());
  std::uniform_real_distribution<float> dist01(0, 1);
  Near::Math::Color color(dist01(rand), dist01(rand), dist01(rand), 1.0f);
  Near::Math::Color white(1.0f, 1.0f, 1.0f, 1.0f);

  uint32_t idx = static_cast<uint32_t>(vertices.size());
  vertices.push_back({from.p3, Near::Math::Vector3::Zero, white, Near::Math::Vector2::Zero});
  vertices.push_back({to.p1, Near::Math::Vector3::Zero, white, Near::Math::Vector2::Zero});
  indices.push_back(idx ++);
  indices.push_back(idx ++);
  for(int i = 0;i < verticesPerSegment;i ++){
    float t = i / static_cast<float>(verticesPerSegment - 1);
    auto pos = CameraPath::Evaluate(t, from.p3, to.p1, to.p2, to.p3);
    vertices.push_back({pos, Near::Math::Vector3::Zero, color, Near::Math::Vector2::Zero});
    if(i == 0) continue;
    indices.push_back(idx ++);
    indices.push_back(idx);
  }
  idx ++;
  vertices.push_back({to.p3, Near::Math::Vector3::Zero, white, Near::Math::Vector2::Zero});
  vertices.push_back({to.p2, Near::Math::Vector3::Zero, white, Near::Math::Vector2::Zero});
  indices.push_back(idx ++);
  indices.push_back(idx ++);
}
