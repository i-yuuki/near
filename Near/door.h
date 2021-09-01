#pragma once

#include <NearLib/collidable.h>
#include <NearLib/fbx-model.h>
#include <NearLib/game-object.h>

#include "activatable.h"

class Door : public Near::GameObject, public Near::ICollidable, public Activatable{
public:
  Door();
  virtual void init(Near::Layer* layer) override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
  virtual void addColliders(std::function<void(const Near::Collision::BoundingBox3D&)> out) override;
protected:
  virtual void activate() override;
  virtual void deactivate() override;
private:
  float time;
  std::unique_ptr<Near::FBXModel> model;
  std::shared_ptr<Near::VertexShader> vertexShader;
  std::shared_ptr<Near::PixelShader> pixelShader;
};
