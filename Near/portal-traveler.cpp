#include "portal-traveler.h"

#include <algorithm>

#include <NearLib/scene.h>

#include "level-object.h"

// 当たり判定が全然うまくできず妥協しまくった結果
// 1回のmove()でくぐれるポータルは1個だけに
// 60fpsもあれば大丈夫だと思うけど
void PortalTraveler::move(Near::Math::Vector3& movement, float deltaTime, std::shared_ptr<Portal> ignorePortal){
  if(movement == Near::Math::Vector3::Zero) return;
  auto scaledMovement = movement * deltaTime;
  float movementLen = scaledMovement.Length();
  Near::Math::Vector3 rayNormalized;
  movement.Normalize(rayNormalized);

  // 使うオブジェクトを探しておく
  std::vector<std::shared_ptr<Portal>> portals;
  getLayer()->getScene()->findObjectsOfExactType<Portal>(portals);
  std::shared_ptr<LevelObject> levelObj = getLayer()->getScene()->findObjectOfExactType<LevelObject>();
  
  // 近くのポータルを探しておく
  std::vector<std::shared_ptr<Portal>> portalsOverlapping;
  Near::Collision::BoundingBox3D me(transform.position, size / 2 + Near::Math::Vector3(DirectX::XMVectorAbs(movement)));
  for(auto& portal : portals){
    Near::Math::Vector3 right = portal->transform.getRight() * portal->getExtents().x;
    Near::Math::Vector3 up    = portal->transform.getUp() * portal->getExtents().y;
    Near::Math::Vector3 portalCorners[] = {
      portal->transform.position - right + up,
      portal->transform.position + right + up,
      portal->transform.position - right - up,
      portal->transform.position + right - up,
    };
    auto portalBox = Near::Collision::BoundingBox3D::FromPoints(portalCorners, 4);
    if(me.intersects(portalBox)){
      portalsOverlapping.push_back(portal);
    }
  }
  
  // 通るポータルを決める
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

  std::shared_ptr<Portal> otherPortal;
  Near::Math::Vector3 posRelativeToOtherPortal;
  Near::Math::Vector3 scaledMovementThroughPortal;
  if(portalToCross){
    otherPortal = portalToCross->otherPortal.lock();
    posRelativeToOtherPortal = TransformThroughPortal(transform, portalToCross, otherPortal);
    scaledMovementThroughPortal = TransformThroughPortal(scaledMovement, portalToCross, otherPortal);
  }

  // レベルと当たり判定
  std::vector<LevelCollision> collisions;
  findCollisions(levelObj->getLevel(), transform.position, scaledMovement, collisions);
  // 近くのポータルの先のレベルとも当たり判定
  for(auto& portal : portalsOverlapping){
    if(auto otherPortal = portal->otherPortal.lock()){
      findCollisions(levelObj->getLevel(), TransformThroughPortal(transform, portal, otherPortal), TransformThroughPortal(scaledMovement, portal, otherPortal), collisions, portal);
    }
  }
  std::sort(collisions.begin(), collisions.end(), [](const LevelCollision& a, const LevelCollision& b){
    return a.hitNear < b.hitNear;
  });

  for(auto& c : collisions){
    auto other = c.throughPortal ? c.throughPortal->otherPortal.lock() : nullptr;
    Near::Math::Vector3 hitPos;
    Near::Math::Vector3 hitDir;
    float hitNear;
    Near::Math::Vector3 pos, ray;
    if(c.throughPortal){
      pos = TransformThroughPortal(transform, c.throughPortal, other);
      ray = TransformThroughPortal(scaledMovement, c.throughPortal, other);
    }else{
      pos = transform.position;
      ray = scaledMovement;
    }
    Near::Collision::BoundingBox3D me(pos, size / 2);
    if(!me.collides(ray, c.aabb, &hitPos, &hitDir, &hitNear)){
      continue;
    }
    // ポータル越しの衝突がポータルの手前だったら当たらない
    // もうここまでくると何がどうなってるのかわからない (課題提出前日23時)
    if(c.throughPortal){
      if(other->transform.getForward().Dot(hitPos - c.throughPortal->transform.position) > 0){
        continue;
      }
    }
    if(hitDir.y >= 0.5f){
      onGround = true;
    }
    // DirectXTKになくてもabsやっちゃうぞ～～
    auto newMovement = movement + hitDir * Near::Math::Vector3(DirectX::XMVectorAbs(DirectX::XMLoadFloat3(&movement))) * (1 - hitNear);
    movement = newMovement;
    scaledMovement = movement * deltaTime;
  }
  
  // ポータルをくぐる portal-scene.cpp 参照
  if(otherPortal){
    Near::Math::Quaternion portalInv;
    portalToCross->transform.rotation.Inverse(portalInv);

    Near::Math::Matrix m = transform.createTransform() * portalToCross->transform.createTransform().Invert() * Near::Math::Matrix::CreateFromYawPitchRoll(DirectX::XM_PI, 0, 0) * otherPortal->transform.createTransform();
    Near::Math::Vector3 dummy;
    m.Decompose(dummy, transform.rotation, transform.position);

    movement = TransformThroughPortal(movement, portalToCross, otherPortal);
  }

  transform.position += movement * deltaTime;
}

void PortalTraveler::findCollisions(const Level* level, Near::Math::Vector3 origin, const Near::Math::Vector3& ray, std::vector<LevelCollision>& out, std::shared_ptr<Portal> portal){
  Near::Collision::BoundingBox3D me(origin, size / 2);
  for(auto& block : level->getBlocks()){
    Near::Collision::BoundingBox3D blockBox(block.position, block.size / 2);
    float hitNear;
    if(me.collides(ray, blockBox, nullptr, nullptr, &hitNear)){
      out.push_back({blockBox, hitNear, portal});
    }
  }
}

const Near::Math::Vector3& PortalTraveler::getSize() const{
  return size;
}

Near::Math::Vector3 PortalTraveler::TransformThroughPortal(const Near::Math::Vector3& pos, std::shared_ptr<Portal> portal, std::shared_ptr<Portal> otherPortal){
  Near::Math::Quaternion portalInv;
  portal->transform.rotation.Inverse(portalInv);
  Near::Math::Vector3 ret = pos;
  ret = Near::Math::Vector3::Transform(ret, portalInv);
  ret = Near::Math::Vector3::Transform(ret, Near::Math::Quaternion::CreateFromYawPitchRoll(DirectX::XM_PI, 0, 0) * otherPortal->transform.rotation);
  return ret;
}

Near::Math::Vector3 PortalTraveler::TransformThroughPortal(const Near::Transform& transform, std::shared_ptr<Portal> portal, std::shared_ptr<Portal> otherPortal){
  Near::Math::Matrix m = transform.createTransform() * portal->transform.createTransform().Invert() * Near::Math::Matrix::CreateFromYawPitchRoll(DirectX::XM_PI, 0, 0) * otherPortal->transform.createTransform();
  Near::Math::Vector3 dummy;
  Near::Math::Quaternion dummyQ;
  Near::Math::Vector3 ret;
  m.Decompose(dummy, dummyQ, ret);
  return ret;
}
