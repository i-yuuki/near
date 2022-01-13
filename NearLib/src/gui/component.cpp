#include "pch.h"
#include "gui/component.h"

#include "near.h"
#include "gui/container.h"

namespace Near::GUI{

Length::Length(float value, Unit unit) : value(value), unit(unit){
}

std::weak_ptr<Component> Component::getParent() const{
  return parent;
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

void Component::setBackground(const Near::Math::Color& background){
  this->background = background;
}

void Component::draw(){
  if(background.A() > 0){
    Near::renderer2D()->setTexture(nullptr);
    Near::renderer2D()->fillRect(layoutPosition, layoutSize, Math::Vector2::Zero, background);
  }
  // else {
	//   background = Near::Math::Color((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 0.5f);

  // }
}

void Component::layout(const BoxConstraints& constraints){
  layoutSize.x = std::max(1.0f, constraints.minWidth);
  layoutSize.y = std::max(1.0f, constraints.minHeight);
}

}
