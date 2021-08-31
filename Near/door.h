#pragma once

#include <NearLib/fbx-model.h>
#include <NearLib/game-object.h>

#include "activatable.h"

class Door : public Near::GameObject, public Activatable{
public:
  Door();
  virtual void init(Near::Layer* layer) override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
protected:
  virtual void activate() override;
  virtual void deactivate() override;
private:
  float time;
  std::unique_ptr<Near::FBXModel> model;
};
