#include "pch.h"
#include "gui/component.h"

#include "near.h"
#include "gui/container.h"

namespace Near::GUI{

Length::Length(float value, Unit unit) : value(value), unit(unit){
}

std::weak_ptr<Container> Component::getParent() const{
  return parent;
}

const Near::Math::Vector2& Component::getPosition() const{
  return position;
}

const Length& Component::getWidth() const{
  return width;
}

const Length& Component::getHeight() const{
  return height;
}

const Near::Math::Vector2& Component::getLayoutPosition() const{
  return layoutPosition;
}

const Near::Math::Vector2& Component::getLayoutSize() const{
  return layoutSize;
}

const Near::Math::Color& Component::getBackground() const{
  return background;
}

void Component::setPosition(const Near::Math::Vector2& position){
  this->position = position;
}

void Component::setWidth(const Length& width){
  this->width = width;
}

void Component::setHeight(const Length& height){
  this->height = height;
}

void Component::setBackground(const Near::Math::Color& background){
  this->background = background;
}

void Component::draw(){
  if(background.A() > 0){
    Near::renderer2D()->setTexture(nullptr);
    Near::renderer2D()->fillRect(layoutPosition, layoutSize, Math::Vector2::Zero, background);
  }
}

void Component::layout(const BoxConstraints& constraints){
  switch(width.unit){
    case Unit::PX:      layoutSize.x = width.value; break;
    case Unit::PERCENT: layoutSize.x = (std::isfinite(constraints.maxWidth) ? constraints.maxWidth : constraints.minWidth) * width.value / 100; break;
    default:            layoutSize.x = width.value; break;
  }
  switch(height.unit){
    case Unit::PX:      layoutSize.y = height.value; break;
    case Unit::PERCENT: layoutSize.y = (std::isfinite(constraints.maxHeight) ? constraints.maxHeight : constraints.minHeight) * height.value / 100; break;
    default:            layoutSize.y = height.value; break;
  }
}

}
