#include "pch.h"
#include "gui/component.h"

#include "near.h"

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
}

void Component::setWidthUnit(SizeUnit unit){
  widthUnit = unit;
}

void Component::setHeightUnit(SizeUnit unit){
  heightUnit = unit;
}

void Component::setBackground(const Near::Math::Color& background){
  this->background = background;
}

void Component::draw(){
  Near::renderer2D()->setTexture(nullptr);
  Near::renderer2D()->fillRect(position, layoutSize, Math::Vector2::Zero, background);
}

void Component::sizeChanged(){
}

}
