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
  std::shared_ptr<Portal> other = otherPortal.lock();
  if(!other) return;

  std::vector<std::shared_ptr<PortalTraveler>> travelers;
  getLayer()->getScene()->findObjectsOfType<PortalTraveler>(travelers);

  Near::Math::Vector3 right   = transform.getRight() * extents.x;
  Near::Math::Vector3 up      = transform.getUp() * extents.y;
  Near::Math::Vector3 p0 = transform.position - right + up;
  Near::Math::Vector3 p1 = transform.position + right + up;
  Near::Math::Vector3 p2 = transform.position - right - up;
  Near::Math::Vector3 p3 = transform.position + right - up;
  for(auto& traveler : travelers){
    if(traveler->transform.position == traveler->lastPosition) continue;
    Near::Math::Vector3 ray = traveler->transform.position - traveler->lastPosition;
    Near::Math::Vector3 rayN;
    ray.Normalize(rayN);
    float t; // のちのち使うはず
    bool intersects = DirectX::TriangleTests::Intersects(
      traveler->lastPosition,
      rayN,
      p0,
      p1,
      p2,
      t
    );
    bool teleport = intersects && t < ray.Length();
    if(!teleport){
      intersects = DirectX::TriangleTests::Intersects(
        traveler->lastPosition,
        rayN,
        p2,
        p1,
        p3,
        t
      );
      teleport = intersects && t < ray.Length();
    }
    if(teleport){
      printf_s("t: %.3f\n", t);
      // portal-scene.cpp 参照
      Near::Math::Quaternion portalInv;
      transform.rotation.Inverse(portalInv);
      Near::Math::Quaternion otherPortalInvOpposite = Near::Math::Quaternion::CreateFromYawPitchRoll(DirectX::XM_PI, 0, 0) * other->transform.rotation;
      Near::Math::Quaternion rot = otherPortalInvOpposite * portalInv;

      traveler->transform.position -= transform.position;
      traveler->transform.position = Near::Math::Vector3::Transform(traveler->transform.position, rot);
      traveler->transform.rotation *= rot;
      traveler->transform.position += other->transform.position;
      continue;
    }
  }
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
