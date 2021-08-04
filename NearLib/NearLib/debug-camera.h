#pragma once

#include "camera.h"

namespace Near{

class DebugCamera : public virtual Camera{
public:
  virtual void update(float deltaTime) override;
private:
  static constexpr float MAX_VELOCITY = 0.15f;
  static constexpr float VELOCITY_ACCELERATION = 0.03f;
  static constexpr float VELOCITY_ATTENUATION = 0.75f;
  static constexpr float ROTATION_VELOCITY_ACCELERATION = 0.006f;
  static constexpr float ROTATION_VELOCITY_ATTENUATION = 0.75f;
  Math::Vector3 velocity{};
  Math::Vector2 rotationVelocity{};
};

}
