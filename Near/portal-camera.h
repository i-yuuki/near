#pragma once


#include <NearLib/camera.h>
#include "portal-traveler.h"

class PortalCamera : public virtual Near::Camera, public virtual PortalTraveler{
public:
  virtual void update(float deltaTime) override;
  bool isDebugControlsEnabled();
  void setDebugControlsEnabled(bool enable);
private:
  bool debugControls;
  float speed = 256.0f;
  float rotationSpeed = 1.5f;
};
