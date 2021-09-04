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
  vertexShader = Near::Assets::vertexShaders()->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = Near::Assets::pixelShaders()->getOrLoad("assets/nearlib/shaders/ps.hlsl");
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

bool Portal::intersects(const Near::Math::Vector3& origin, const Near::Math::Vector3& ray, float* dist){
  Near::Math::Vector3 right = transform.getRight() * extents.x;
  Near::Math::Vector3 up    = transform.getUp() * extents.y;
  Near::Math::Vector3 p0    = transform.position - right + up;
  Near::Math::Vector3 p1    = transform.position + right + up;
  Near::Math::Vector3 p2    = transform.position - right - up;
  Near::Math::Vector3 p3    = transform.position + right - up;
  Near::Math::Vector3 rayNormalized;
  ray.Normalize(rayNormalized);
  float len = ray.Length();
  float t;
  bool intersects = DirectX::TriangleTests::Intersects(
    origin,
    rayNormalized,
    p0, p1, p2,
    t
  );
  bool ret = intersects && t < len && t > 0;
  if(!ret){
    intersects = DirectX::TriangleTests::Intersects(
      origin,
      rayNormalized,
      p2, p1, p3,
      t
    );
    ret = intersects && t < len && t > 0;
  }
  if(ret && dist){
    *dist = t;
  }
  return ret;
}
