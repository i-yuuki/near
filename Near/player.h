#pragma once

#include <NearLib/game-object.h>
#include <NearLib/fbx-model.h>

#include "cube.h"
#include "portal-traveler.h"
#include "level.h"

class Player : public virtual PortalTraveler, public Near::ICollidable{
public:
  virtual void init(Near::Layer* layer) override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
  virtual void addColliders(std::function<void(const Near::Collision::BoundingBox3D&)> out) override;
  bool isThirdPerson() const;
  void interact();
private:
  Near::Event::ListenerPtr<Near::InputManager::KeyEvent> keyListener;
  Near::Math::Vector2 movement;
  Near::Math::Vector3 velocity;
  bool thirdPerson;
  std::weak_ptr<Cube> cubeHolding;
};
