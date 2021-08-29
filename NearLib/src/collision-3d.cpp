#include "pch.h"
#include "collision-3d.h"

#include <algorithm>

namespace Near::Collision{

Ray3D::Ray3D(const Math::Vector3& position, const Math::Vector3& ray)
  : position(position), ray(ray){
}

bool Ray3D::collides(const BoundingBox3D& box, Math::Vector3* hitPoint, Math::Vector3* hitDir, float* hitNear) const{
  Math::Vector3 boxMin = box.getCenter() - box.getExtents();
  Math::Vector3 boxMax = box.getCenter() + box.getExtents();
  Math::Vector3 posNear = (boxMin - position) / ray;
  Math::Vector3 posFar  = (boxMax - position) / ray;
  // Math::Vector3 posNear((boxMin.x - position.x) / ray.x, (boxMin.y - position.y) / ray.y, (boxMin.z - position.z) / ray.z);
  // Math::Vector3 posFar((boxMax.x - position.x) / ray.x, (boxMax.y - position.y) / ray.y, (boxMax.z - position.z) / ray.z);

  if(std::isnan(posNear.x) || std::isnan(posNear.y) || std::isnan(posFar.x) || std::isnan(posFar.y)) return false;

  if(posNear.x > posFar.x) std::swap(posNear.x, posFar.x);
  if(posNear.y > posFar.y) std::swap(posNear.y, posFar.y);
  if(posNear.z > posFar.z) std::swap(posNear.z, posFar.z);

  float tNear = std::max({posNear.x, posNear.y, posNear.z});
  float tFar = std::min({posFar.x, posFar.y, posFar.z});

  if(posNear.x > posFar.y || posNear.y > posFar.x
  || posNear.z > posFar.y || posNear.y > posFar.z
  || posNear.x > posFar.z || posNear.z > posFar.x) return false;

  if(hitNear){
    *hitNear = tNear;
  }
  if(tFar < -0.001f || tNear < -0.001f) return false; // rがdirectionの正反対の方向にある
  if(hitPoint){
    hitPoint->x = position.x + tNear * ray.x;
    hitPoint->y = position.y + tNear * ray.y;
    hitPoint->z = position.z + tNear * ray.z;
  }

  if(hitDir){
    if(posNear.x > std::max(posNear.y, posNear.z)){
      hitDir->x = ray.x < 0 ? 1.0f : -1.0f;
      hitDir->y = 0;
      hitDir->z = 0;
    }else if(posNear.y > std::max(posNear.x, posNear.z)){
      hitDir->x = 0;
      hitDir->y = ray.y < 0 ? 1.0f : -1.0f;
      hitDir->z = 0;
    }else if(posNear.z > std::max(posNear.x, posNear.y)){
      hitDir->x = 0;
      hitDir->y = 0;
      hitDir->z = ray.z < 0 ? 1.0f : -1.0f;
    }
  }

  return true;
}

BoundingBox3D::BoundingBox3D(){
}

BoundingBox3D::BoundingBox3D(const Math::Vector3& center, const Math::Vector3& extents)
  : center(center), extents(extents){
}

const Math::Vector3& BoundingBox3D::getCenter() const{
  return center;
}

const Math::Vector3& BoundingBox3D::getExtents() const{
  return extents;
}

bool BoundingBox3D::intersects(const BoundingBox3D& other) const{
  auto min = center - extents;
  auto max = center + extents;
  auto otherMin = other.center - other.extents;
  auto otherMax = other.center + other.extents;
  return min.x < otherMax.x && otherMin.x < max.x && min.y < otherMax.y && otherMin.y < max.y && min.z < otherMax.z && otherMin.z < max.z;
}

bool BoundingBox3D::collides(const Math::Vector3& velocity, const BoundingBox3D& other, Math::Vector3* hitPoint, Math::Vector3* hitDir, float* hitNear) const{
  if(velocity == Math::Vector3::Zero) return false;
  BoundingBox3D otherExpanded = other;
  otherExpanded.extents += extents;
  float near_ = 0;
  bool collides = Ray3D(center, velocity).collides(otherExpanded, hitPoint, hitDir, &near_);
  if(hitNear){
    *hitNear = near_;
  }

  return collides && near_ <= 1;
}

BoundingBox3D BoundingBox3D::FromPoints(const Math::Vector3* points, size_t count){
  if(count == 0) throw std::exception("No points to create BoundingBox3D");
  Near::Math::Vector3 min = points[0];
  Near::Math::Vector3 max = min;
  for(size_t i = 1;i < count;i ++){
    min = Near::Math::Vector3::Min(min, points[i]);
    max = Near::Math::Vector3::Max(max, points[i]);
  }
  return BoundingBox3D(Near::Math::Vector3::Lerp(min, max, 0.5f), (max - min) / 2);
}

}
