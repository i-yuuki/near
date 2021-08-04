#pragma once


#include <NearLib/debug-camera.h>
#include "portal-traveler.h"

class PortalCamera : public virtual Near::DebugCamera, public virtual PortalTraveler{
public:
  virtual void update(float deltaTime) override;
  // 複数継承の警告を黙らせる (usingは効かなかった･_･)
  virtual void draw() override{ Near::DebugCamera::draw(); }
  // #pragma warning(suppress: 4250)
};
