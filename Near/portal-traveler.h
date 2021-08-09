#pragma once

#include <NearLib/game-object.h>

#include "portal.h"

class PortalTraveler : public virtual Near::GameObject{
public:
  void move(Near::Math::Vector3 movement, std::shared_ptr<Portal> ignorePortal = nullptr);
  const Near::Math::Vector3& getSize() const;
protected:
  using super = Near::GameObject;
  Near::Math::Vector3 size;
  bool onGround;
};
