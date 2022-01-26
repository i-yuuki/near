#pragma once

#include "game-object.h"
#include "gui/component.h"

namespace Near{

class GUIObject : public GameObject{
public:
  GUIObject(std::shared_ptr<GUI::Component> gui);
  virtual void init(Near::Layer* layer) override;
  virtual void draw() override;
  virtual void uninit() override;
private:
  std::shared_ptr<GUI::Component> gui;
  Event::ListenerPtr<InputManager::MouseEvent> mouseDownListener;
};

}
