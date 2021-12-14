#pragma once

#include "game-object.h"
#include "gui/component.h"

namespace Near{

class GUIObject : public GameObject{
public:
  GUIObject(std::shared_ptr<GUI::Component> gui);
  virtual void draw();
private:
  std::shared_ptr<GUI::Component> gui;
};

}
