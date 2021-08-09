#include "portal.h"

#include <NearLib/vertex.h>
#include <NearLib/scene.h>
#include "portal-traveler.h"

Portal::Portal(Near::Math::Vector2 extents, Near::Math::Color color) : extents(extents), color(color){
}

void Portal::init(Near::Layer* layer){
  super::init(layer);
  texture.load(u8"assets/textures/portal.png");
  // texture1.load(u8"assets/textures/throw.png");
  Near::Vertex3D vertices[4] = {
    {Near::Math::Vector3(-extents.x,  extents.y, 0), Near::Math::Vector3(0, 0, 1), color, Near::Math::Vector2(0, 0)},
    {Near::Math::Vector3( extents.x,  extents.y, 0), Near::Math::Vector3(0, 0, 1), color, Near::Math::Vector2(1, 0)},
    {Near::Math::Vector3(-extents.x, -extents.y, 0), Near::Math::Vector3(0, 0, 1), color, Near::Math::Vector2(0, 1)},
    {Near::Math::Vector3( extents.x, -extents.y, 0), Near::Math::Vector3(0, 0, 1), color, Near::Math::Vector2(1, 1)},
  };
  vertexBuffer.init(false, 4, vertices);
  vertexShader = layer->getScene()->vertexShaders->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = layer->getScene()->pixelShaders->getOrLoad("assets/nearlib/shaders/ps.hlsl");
}

void Portal::update(float deltaTime){
  super::update(deltaTime);
}

void Portal::draw(){
  auto* renderer = Near::renderer();
  renderer->setVertexShader(vertexShader.get());
  renderer->setPixelShader(pixelShader.get());
  renderer->setTexture(&texture);
  renderer->pushWorldTransform();
  renderer->applyWorldTransform(transform.createTransform());
  vertexBuffer.draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  // renderer->setTexture(&texture1);
  // renderer->applyWorldTransform(Near::Math::Matrix::CreateTranslation(0.5f, 0.5f, 0) * Near::Math::Matrix::CreateScale(0.5f, 0.5f * (550.0f / 800.0f), 1));
  // vertexBuffer.draw(sizeof(Near::Vertex3D), 0, 4, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  renderer->popWorldTransform();
}

const Near::Math::Vector2& Portal::getExtents() const{
  return extents;
}
