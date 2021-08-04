#pragma once

#include "near.h"

namespace Near::Collision{

class Ray3D;
class BoundingBox3D;

class Ray3D{
public:
  Ray3D(const Math::Vector3& position, const Math::Vector3& ray);
  bool collides(const BoundingBox3D& box, Math::Vector3* hitPoint, Math::Vector3* hitDir, float* hitNear) const;
private:
  Math::Vector3 position;
  Math::Vector3 ray;
};

class BoundingBox3D{
public:
  BoundingBox3D();
  BoundingBox3D(const Math::Vector3& center, const Math::Vector3& extents);
  const Math::Vector3& getCenter() const;
  const Math::Vector3& getExtents() const;
  bool collides(const Math::Vector3& velocity, const BoundingBox3D& other, Math::Vector3* hitPoint, Math::Vector3* hitDir, float* hitNear) const;
private:
  Math::Vector3 center;
  Math::Vector3 extents;
};

}
