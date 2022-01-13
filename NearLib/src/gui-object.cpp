#include "pch.h"
#include "gui-object.h"

namespace Near{

GUIObject::GUIObject(std::shared_ptr<GUI::Component> gui) : gui(gui){
}

void GUIObject::init(Near::Layer* layer){
  auto* r = Near::renderer();
  gui->layout(GUI::BoxConstraints().setWidth(r->getWidth()).setHeight(r->getHeight()));
}

void GUIObject::draw(){
  auto* r2d = renderer2D();
  r2d->begin();
  gui->draw();
  r2d->end();
}

}
