#include "pch.h"
#include "debug-grid.h"

#include "scene.h"
#include "camera.h"
#include "utils.h"

namespace Near{

DebugGrid::DebugGrid(unsigned int lineCount) : lineCount(lineCount){
  vertexCount = (lineCount * 2 + 1) * 4;
}

void DebugGrid::init(Layer* layer){
  GameObject::init(layer);
  vertices = new Vertex3D[vertexCount];
  vertexBuffer.init(true, vertexCount * sizeof(Vertex3D), nullptr);
  updateVertexBuffer(true);
  vertexShader = layer->getScene()->vertexShaders->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = layer->getScene()->pixelShaders->getOrLoad("assets/nearlib/shaders/ps.hlsl");
}

void DebugGrid::draw(){
  updateVertexBuffer();
  auto* r = renderer();
  r->setVertexShader(vertexShader.get());
  r->setPixelShader(pixelShader.get());
  r->pushWorldTransform();
  r->setWorldTransform(Math::Matrix::Identity);
  vertexBuffer.draw(sizeof(Vertex3D), 0, vertexCount, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
  r->popWorldTransform();
}

void DebugGrid::uninit(){
  safeDeleteArray(vertices);
}

void DebugGrid::updateVertexBuffer(bool force){
  auto camera = getLayer()->getScene()->findObjectOfType<Camera>();
  int originX, originY, originZ;
  if(camera){
    originX = static_cast<int>(std::round(camera->transform.position.x));
    originY = static_cast<int>(std::round(camera->transform.position.y));
    originZ = static_cast<int>(std::round(camera->transform.position.z));
  }else{
    originX = 0;
    originY = 0;
    originZ = 0;
  }
  if(!force && originX == lastOriginX && originY == lastOriginY && originZ == lastOriginZ) return;
  lastOriginX = originX;
  lastOriginY = originY;
  lastOriginZ = originZ;

  int vi = 0;
  for(unsigned int i = 0;i <= lineCount * 2;i ++){
    int z = originZ - lineCount + i;
    vertices[vi ++] = {Math::Vector3(originX - static_cast<float>(lineCount), 0, z), Math::Vector3::Zero, z == 0 ? Math::Color(1, 0, 0, 1) : Math::Color(1, 1, 1, 1), Math::Vector2::Zero};
    vertices[vi ++] = {Math::Vector3(originX + static_cast<float>(lineCount), 0, z), Math::Vector3::Zero, z == 0 ? Math::Color(1, 0, 0, 1) : Math::Color(1, 1, 1, 1), Math::Vector2::Zero};
  }
  for(unsigned int i = 0;i <= lineCount * 2;i ++){
    int x = originX - lineCount + i;
    vertices[vi ++] = {Math::Vector3(x, 0, originZ - static_cast<float>(lineCount)), Math::Vector3::Zero, x == 0 ? Math::Color(0, 0, 1, 1) : Math::Color(1, 1, 1, 1), Math::Vector2::Zero};
    vertices[vi ++] = {Math::Vector3(x, 0, originZ + static_cast<float>(lineCount)), Math::Vector3::Zero, x == 0 ? Math::Color(0, 0, 1, 1) : Math::Color(1, 1, 1, 1), Math::Vector2::Zero};
  }
  vertexBuffer.set(vertices);
}

}
