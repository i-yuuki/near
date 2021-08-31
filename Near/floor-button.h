#pragma once

#include <NearLib/fbx-model.h>
#include <NearLib/game-object.h>

#include "activatable.h"

class FloorButton : public Near::GameObject, public Activatable{
public:
  FloorButton();
  virtual void init(Near::Layer* layer) override;
  virtual void draw() override;
  virtual void uninit() override;
protected:
  virtual void activate() override;
  virtual void deactivate() override;
private:
  std::unique_ptr<Near::FBXModel> model;
};
