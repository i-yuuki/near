#include "pch.h"
#include "gui/component.h"

#include "near.h"
#include "gui/container.h"

namespace Near::GUI{

std::weak_ptr<Container> Component::getParent() const{
  return parent;
}

const Near::Math::Vector2& Component::getPosition() const{
  return position;
}

const Near::Math::Vector2& Component::getSize() const{
  return size;
}

const Near::Math::Vector2& Component::getLayoutPosition() const{
  return layoutPosition;
}

const Near::Math::Vector2& Component::getLayoutSize() const{
  return layoutSize;
}

SizeUnit Component::getWidthUnit() const{
  return widthUnit;
}

SizeUnit Component::getHeightUnit() const{
  return heightUnit;
}

const Near::Math::Color& Component::getBackground() const{
  return background;
}

void Component::setPosition(const Near::Math::Vector2& position){
  this->position = position;
}

void Component::setSize(const Near::Math::Vector2& size){
  this->size = size;
  sizeChanged();
}

void Component::setWidthUnit(SizeUnit unit){
  widthUnit = unit;
  sizeChanged();
}

void Component::setHeightUnit(SizeUnit unit){
  heightUnit = unit;
  sizeChanged();
}

void Component::setBackground(const Near::Math::Color& background){
  this->background = background;
}

void Component::draw(){
  Near::renderer2D()->setTexture(nullptr);
  Near::renderer2D()->fillRect(layoutPosition, layoutSize, Math::Vector2::Zero, background);
}

void Component::layout(){
  layoutPosition = position;
  computeSize();
}

void Component::layoutParent(){
  if(auto p = parent.lock()){
    p->layout();
  }
}

void Component::computeSize(){
  Near::Math::Vector2 parentSize;
  if(auto p = parent.lock()){
    parentSize = p->getLayoutSize();
  }else{
    auto* r = Near::renderer();
    parentSize = Near::Math::Vector2(r->getWidth(), r->getHeight());
  }
  switch(getWidthUnit()){
    case SizeUnit::FILL_CONTAINER: break; // 親のflex-containerに決めさせる
    case SizeUnit::PARENT:         layoutSize.x = parentSize.x * size.x; break;
    default:                       layoutSize.x = size.x; break;
  }
  switch(getHeightUnit()){
    case SizeUnit::FILL_CONTAINER: break; // 親のflex-containerに決めさせる
    case SizeUnit::PARENT:         layoutSize.y = parentSize.y * size.y; break;
    default:                       layoutSize.y = size.y; break;
  }
}

void Component::sizeChanged(){
  layoutParent();
}

}
