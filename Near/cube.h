#pragma once

#include <NearLib/fbx-model.h>

#include "floor-button.h"
#include "portal-traveler.h"

class Player;

class Cube : public PortalTraveler, public Near::ICollidable{
public:
  Cube();
  virtual void init(Near::Layer* layer) override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
  virtual void addColliders(std::function<void(const Near::Collision::BoundingBox3D&)> out) override;
  void setHolder(Player* holder);
private:
  Player* holder = nullptr;
  std::weak_ptr<FloorButton> button;
  Near::Math::Vector3 velocity;
  std::shared_ptr<Near::FBXModel> model;
  std::shared_ptr<Near::VertexShader> vertexShader;
  std::shared_ptr<Near::PixelShader> pixelShader;
};
