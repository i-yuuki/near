#pragma once

#include <NearLib/camera.h>

#include "player.h"

class PlayerCamera : public virtual Near::Camera{
public:
  PlayerCamera(std::weak_ptr<Player> player);
  virtual void afterUpdate(float deltaTime) override;
private:
  std::weak_ptr<Player> player;
  bool isThirdPerson = false;
};
