#pragma once

#include <NearLib/fbx-model.h>

#include "floor-button.h"
#include "portal-traveler.h"

class Player;

class Cube : public PortalTraveler{
public:
  Cube();
  virtual void init(Near::Layer* layer) override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
  void setHolder(Player* holder);
private:
  Player* holder = nullptr;
  std::weak_ptr<FloorButton> button;
  Near::Math::Vector3 velocity;
  std::shared_ptr<Near::FBXModel> model;
};
