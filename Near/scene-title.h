#pragma once

#include <NearLib/camera.h>

#include "level.h"
#include "polygon-2d.h"
#include "portal-scene.h"

class SceneTitle : public PortalScene{
public:
  SceneTitle();
  virtual void init() override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
private:
  std::shared_ptr<Level> level;
  std::shared_ptr<Near::Camera> camera;
  std::shared_ptr<Polygon2D> title;
  float time;
};
