#pragma once

#include <NearLib/collision-3d.h>
#include <NearLib/game-object.h>

#include "portal.h"
#include "level-object.h"

class PortalTraveler : public virtual Near::GameObject{
public:
  void move(Near::Math::Vector3& movement, float deltaTime, std::shared_ptr<Portal> ignorePortal = nullptr);
  const Near::Math::Vector3& getSize() const;
protected:
  struct LevelCollision{
    Near::Collision::BoundingBox3D aabb;
    float hitNear;
    std::shared_ptr<Portal> throughPortal;
  };
  using super = Near::GameObject;
  Near::Math::Vector3 size;
  bool onGround;
  void findCollisions(const Level* level, Near::Math::Vector3 origin, const Near::Math::Vector3& ray, std::vector<LevelCollision>& out, std::shared_ptr<Portal> portal = nullptr);
  static Near::Math::Vector3 TransformThroughPortal(const Near::Math::Vector3& pos, std::shared_ptr<Portal> portal, std::shared_ptr<Portal> otherPortal);
  static Near::Math::Vector3 TransformThroughPortal(const Near::Transform& transform, std::shared_ptr<Portal> portal, std::shared_ptr<Portal> otherPortal);
};
