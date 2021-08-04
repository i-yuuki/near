#pragma once

#include "game-object.h"

namespace Near{

// draw()でレンダラーの変換行列を更新するオブジェクト
class Camera : public virtual GameObject{
public:
  float getFOV();
  void setFOV(float degrees);
  virtual void draw() override;
  Math::Matrix createViewTransform();
  Math::Matrix createProjectionTransform();
private:
  float fov = 90;
  float nearPlane = 0.1f;
  float farPlane = 100;
};

}
