#include "polygon-2d.h"

#include <NearLib/scene.h>

Polygon2D::Polygon2D(const std::string& path, Near::Math::Vector2 position, Near::Math::Vector2 size)
: Near::GameObject(), path(path), position(position), size(size){
}

void Polygon2D::init(Near::Layer* layer){
  Near::GameObject::init(layer);
  vertices[0] = {Near::Math::Vector3(position.x,          position.y + size.y, 0), Near::Math::Vector3(0, 0, 0), Near::Math::Color(1, 1, 1, 1), Near::Math::Vector2(0, 1)};
  vertices[1] = {Near::Math::Vector3(position.x + size.x, position.y + size.y, 0), Near::Math::Vector3(0, 0, 0), Near::Math::Color(1, 1, 1, 1), Near::Math::Vector2(1, 1)};
  vertices[2] = {Near::Math::Vector3(position.x,          position.y,          0), Near::Math::Vector3(0, 0, 0), Near::Math::Color(1, 1, 1, 1), Near::Math::Vector2(0, 0)};
  vertices[3] = {Near::Math::Vector3(position.x + size.x, position.y,          0), Near::Math::Vector3(0, 0, 0), Near::Math::Color(1, 1, 1, 1), Near::Math::Vector2(1, 0)};
  vertexBuffer.init(true, 4, vertices);
  texture = Near::Assets::textures()->getOrLoad(path);
  vertexShader = Near::Assets::vertexShaders()->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = Near::Assets::pixelShaders()->getOrLoad("assets/nearlib/shaders/ps.hlsl");
}

void Polygon2D::draw(){
  auto* r = Near::renderer();
  r->setVertexShader(vertexShader.get());
  r->setPixelShader(pixelShader.get());
  r->setTexture(texture.get());
  r->setWorldTransform(Near::Math::Matrix::Identity);
  r->setViewTransform(Near::Math::Matrix::Identity);
  r->setProjectionTransform(Near::Math::Matrix::CreateOrthographicOffCenter(0, r->getWidth(), r->getHeight(), 0, 0, 1));
  vertexBuffer.draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Polygon2D::uninit(){
  vertexBuffer.uninit();
  texture.reset();
  vertexShader.reset();
  pixelShader.reset();
}

void Polygon2D::setTexture(std::shared_ptr<Near::Texture> texture){
  this->texture = texture;
}

void Polygon2D::setColor(const Near::Math::Color& color){
  for(int i = 0;i < 4;i ++){
    vertices[i].color = color;
  }
  vertexBuffer.set(vertices);
}
