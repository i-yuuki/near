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

void PortalTraveler::move(Near::Math::Vector3 movement, std::shared_ptr<Portal> ignorePortal){
  if(movement == Near::Math::Vector3::Zero) return;
  float movementLen = movement.Length();
  Near::Math::Vector3 rayNormalized;
  movement.Normalize(rayNormalized);

  // 一番近いポータルを探しておく
  std::vector<std::shared_ptr<Portal>> portals;
  getLayer()->getScene()->findObjectsOfExactType<Portal>(portals);
  std::shared_ptr<Portal> portalToCross;
  float portalDistance;
  for(auto& portal : portals){
    if(portal == ignorePortal) continue;
    auto& extents = portal->getExtents();
    Near::Math::Vector3 right = portal->transform.getRight() * extents.x;
    Near::Math::Vector3 up    = portal->transform.getUp() * extents.y;
    Near::Math::Vector3 p0 = portal->transform.position - right + up;
    Near::Math::Vector3 p1 = portal->transform.position + right + up;
    Near::Math::Vector3 p2 = portal->transform.position - right - up;
    Near::Math::Vector3 p3 = portal->transform.position + right - up;

    float t;
    bool intersects = DirectX::TriangleTests::Intersects(
      transform.position,
      rayNormalized,
      p0, p1, p2,
      t
    );
    bool canTeleport = intersects && t < movementLen && t > 0;
    if(!canTeleport){
      intersects = DirectX::TriangleTests::Intersects(
        transform.position,
        rayNormalized,
        p2, p1, p3,
        t
      );
      canTeleport = intersects && t < movementLen && t > 0;
    }
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
    if(me.collides(movement, blockBox, nullptr, nullptr, &hitNear)){
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
    if(!me.collides(movement, c.aabb, &hitPos, &hitDir, &hitNear)){
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
  }
  
  // ポータルをくぐる portal-scene.cpp 参照
  if(otherPortal){
    Near::Math::Vector3 movementUntilPortal = rayNormalized * portalDistance;
    transform.position += movementUntilPortal;

    Near::Math::Quaternion portalInv;
    portalToCross->transform.rotation.Inverse(portalInv);
    Near::Math::Quaternion otherPortalInvOpposite = Near::Math::Quaternion::CreateFromYawPitchRoll(DirectX::XM_PI, 0, 0) * otherPortal->transform.rotation;
    Near::Math::Quaternion rot = otherPortalInvOpposite * portalInv;

    transform.position -= portalToCross->transform.position;
    transform.position = Near::Math::Vector3::Transform(transform.position, rot);
    transform.rotation *= rot;
    transform.position += otherPortal->transform.position;

    // ポータルの先の当たり判定
    // (くぐったポータルを戻ってきて無限再帰する？ので無視させつつ)
    move(movement - movementUntilPortal, otherPortal);
    return;
  }

  transform.position += movement;
}

const Near::Math::Vector3& PortalTraveler::getSize() const{
  return size;
}
