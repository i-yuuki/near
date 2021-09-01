#pragma once

#include <functional>

#include "collision-3d.h"

namespace Near{

class ICollidable{
public:
  virtual void addColliders(std::function<void(const Collision::BoundingBox3D&)> out) = 0;
};

}
