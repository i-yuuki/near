#pragma once

#include <NearLib/fbx-model.h>

#include "portal-traveler.h"

class Player;

class Cube : public PortalTraveler{
public:
  Cube();
  virtual void init(Near::Layer* layer) override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
private:
  std::weak_ptr<Player> holder;
  Near::Math::Vector3 velocity;
  std::shared_ptr<Near::FBXModel> model;
};
