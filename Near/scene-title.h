#pragma once

#include "level.h"
#include "polygon-2d.h"
#include "portal-scene.h"
#include "portal-camera.h"
#include "camera-path.h"
#include "camera-path-object.h"

class SceneTitle : public PortalScene{
public:
  SceneTitle();
  virtual void init() override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
private:
  std::shared_ptr<Level> level;
  std::shared_ptr<PortalCamera> camera;
  std::shared_ptr<Polygon2D> title;
  float time;
};
