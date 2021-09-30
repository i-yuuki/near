#include "pch.h"
#include "gui/component.h"

#include "near.h"

namespace Near::GUI{

std::weak_ptr<Component> Component::getParent(){
  return parent;
}

const Near::Math::Vector2& Component::getPosition(){
  return position;
}

const Near::Math::Vector2& Component::getSize(){
  return size;
}

SizeUnit Component::getWidthUnit(){
  return widthUnit;
}

SizeUnit Component::getHeightUnit(){
  return heightUnit;
}

const Near::Math::Color& Component::getBackground(){
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
