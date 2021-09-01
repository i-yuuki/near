#pragma once

#include "polygon-2d.h"
#include "portal-scene.h"

class SceneFinish : public Near::Scene{
public:
  SceneFinish();
  virtual void init() override;
  virtual void update(float deltaTime) override;
};
