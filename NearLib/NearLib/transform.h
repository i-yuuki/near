#pragma once

#include "near.h"

namespace Near{

class Transform{
public:
  Math::Vector3 getForward();
  Math::Vector3 getRight();
  Math::Vector3 getUp();
  Math::Matrix createTransform();
  Math::Vector3 position = Math::Vector3(0.0f);
  Math::Quaternion rotation = Math::Quaternion::Identity;
  Math::Vector3 scale = Math::Vector3(1.0f);
};

}
