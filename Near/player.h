#pragma once

#include <NearLib/game-object.h>
#include <NearLib/fbx-model.h>

#include "portal-traveler.h"
#include "level.h"

class Player : public virtual PortalTraveler, public Near::KeyListener{
public:
  virtual void init(Near::Layer* layer) override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
  virtual void onKeyDown(int vkey, bool isRepeat) override;
  bool isThirdPerson() const;
private:
  Near::InputConnection keyListener;
  Near::Math::Vector2 movement;
  Near::Math::Vector3 velocity;
  float yaw;
  float pitch;
  bool thirdPerson;
  Near::FBXModel* model;
  std::shared_ptr<Near::VertexShader> vertexShader;
  std::shared_ptr<Near::PixelShader> pixelShader;
};
