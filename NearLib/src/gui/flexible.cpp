#include "pch.h"
#include "gui/flexible.h"

#include "near.h"

namespace Near::GUI{

Flexible::Flexible(float flex, std::shared_ptr<Component> child) : flex(flex), child(child){
}

float Flexible::getFlex() const{
  return flex;
}

void Flexible::setFlex(float flex){
  this->flex = flex;
}

void Flexible::layout(const BoxConstraints& constraints){
  Component::layout(constraints);
  if(!child) return;
  child->layout(BoxConstraints(layoutSize, layoutSize));
  layoutSize = child->getLayoutSize();
}

void Flexible::draw(){
  Component::draw();
  if(!child) return;
  renderer2D()->pushTransform();
  renderer2D()->translate(layoutPosition.x, layoutPosition.y);
  child->draw();
  renderer2D()->popTransform();
}

}
