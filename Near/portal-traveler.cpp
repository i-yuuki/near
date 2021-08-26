#include "portal-traveler.h"

#include <algorithm>

#include <NearLib/collision-3d.h>
#include <NearLib/scene.h>

#include "level-object.h"

namespace{

struct LevelCollision{
  Near::Collision::BoundingBox3D aabb;
  float hitNear;
};

}

void PortalTraveler::move(Near::Math::Vector3& movement, float deltaTime, std::shared_ptr<Portal> ignorePortal){
  if(movement == Near::Math::Vector3::Zero) return;
  auto scaledMovement = movement * deltaTime;
  float movementLen = scaledMovement.Length();
  Near::Math::Vector3 rayNormalized;
  movement.Normalize(rayNormalized);

  // 一番近いポータルを探しておく
  std::vector<std::shared_ptr<Portal>> portals;
  getLayer()->getScene()->findObjectsOfExactType<Portal>(portals);
  std::shared_ptr<Portal> portalToCross;
  float portalDistance;
  for(auto& portal : portals){
    if(portal == ignorePortal) continue;

    float t;
    bool canTeleport = portal->intersects(transform.position, scaledMovement, &t);
    if(canTeleport && (!portalToCross || t < portalDistance)){
      portalToCross = portal;
      portalDistance = t;
    }
  }
  std::shared_ptr<Portal> otherPortal = portalToCross ? portalToCross->otherPortal.lock() : nullptr;

  // レベルと当たり判定
  auto levelObj = getLayer()->getScene()->findObjectOfExactType<LevelObject>();
  std::vector<LevelCollision> collisions;
  Near::Collision::BoundingBox3D me(transform.position, size / 2);
  for(auto& block : levelObj->getLevel()->getBlocks()){
    Near::Collision::BoundingBox3D blockBox(block.position, block.size / 2);
    float hitNear;
    if(me.collides(scaledMovement, blockBox, nullptr, nullptr, &hitNear)){
      collisions.push_back({blockBox, hitNear});
    }
  }
  std::sort(collisions.begin(), collisions.end(), [](const LevelCollision& a, const LevelCollision& b){
    return a.hitNear < b.hitNear;
  });

  for(auto& c : collisions){
    Near::Math::Vector3 hitPos;
    Near::Math::Vector3 hitDir;
    float hitNear;
    if(!me.collides(scaledMovement, c.aabb, &hitPos, &hitDir, &hitNear)){
      continue;
    }
    if(hitDir.y >= 0.5f){
      onGround = true;
    }
    // DirectXTKになくてもabsやっちゃうぞ～～
    auto newMovement = movement + hitDir * Near::Math::Vector3(DirectX::XMVectorAbs(DirectX::XMLoadFloat3(&movement))) * (1 - hitNear);
    if(otherPortal && portalToCross->transform.getForward().Dot(hitPos - portalToCross->transform.position) < 0){
      // ポータルの先のブロック、もう当たり判定しない
      break;
    }
    movement = newMovement;
    scaledMovement = movement * deltaTime;
  }
  
  // ポータルをくぐる portal-scene.cpp 参照
  if(otherPortal){
    Near::Math::Vector3 movementUntilPortal = rayNormalized * portalDistance;
    transform.position += movementUntilPortal;

    Near::Math::Quaternion portalInv;
    portalToCross->transform.rotation.Inverse(portalInv);

    Near::Math::Matrix m = transform.createTransform() * portalToCross->transform.createTransform().Invert() * Near::Math::Matrix::CreateFromYawPitchRoll(DirectX::XM_PI, 0, 0) * otherPortal->transform.createTransform();
    Near::Math::Vector3 dummy;
    m.Decompose(dummy, transform.rotation, transform.position);

    // ポータルの先の当たり判定
    // (くぐったポータルを戻ってきて無限再帰する？ので無視させつつ)
    movement = Near::Math::Vector3::Transform(movement, portalInv);
    movement = Near::Math::Vector3::Transform(movement, Near::Math::Quaternion::CreateFromYawPitchRoll(DirectX::XM_PI, 0, 0) * otherPortal->transform.rotation);
    move(movement, deltaTime * (movementLen - portalDistance), otherPortal);
    return;
  }

  transform.position += movement * deltaTime;
}

const Near::Math::Vector3& PortalTraveler::getSize() const{
  return size;
}
