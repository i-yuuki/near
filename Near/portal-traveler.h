#pragma once

#include <NearLib/game-object.h>

class PortalTraveler : public virtual Near::GameObject{
public:
  virtual void update(float deltaTime) override;
  Near::Math::Vector3 lastPosition;
protected:
  using super = Near::GameObject;
};
