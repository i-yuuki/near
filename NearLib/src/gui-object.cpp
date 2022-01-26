#include "pch.h"
#include "gui-object.h"

namespace Near{

GUIObject::GUIObject(std::shared_ptr<GUI::Component> gui) : gui(gui){
}

void GUIObject::init(Near::Layer* layer){
  auto* r = Near::renderer();
  gui->layout(GUI::BoxConstraints().setWidth(r->getWidth()).setHeight(r->getHeight()));
  mouseDownListener = input()->onMouseDown.addListener([this](InputManager::MouseEvent e){
    Near::Math::Vector2 mousePos(e.x, e.y);
    if(auto c = gui->getDeepComponentAt(mousePos)){
      c->propagateMouseDownEvent(Near::GUI::MouseEvent(c, mousePos, e.button));
    }
  });
}

void GUIObject::draw(){
  auto* r2d = renderer2D();
  r2d->begin();
  gui->draw();
  r2d->end();
}

void GUIObject::uninit(){
  mouseDownListener->disconnect();
}

}
