#pragma once

#include "portal-scene.h"
#include "level.h"

class SceneGame : public PortalScene{
public:
  SceneGame();
  virtual void init() override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
private:
  std::shared_ptr<Level> level;
};
