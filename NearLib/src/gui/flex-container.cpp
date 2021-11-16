#include "pch.h"
#include "gui/flex-container.h"

#include "near.h"

namespace Near::GUI{

FlexContainer::FlexContainer(Direction direction) : direction(direction){
}

FlexContainer::Direction FlexContainer::getDirection() const{
  return direction;
}

void FlexContainer::setDirection(Direction direction){
  if(direction != this->direction){
    this->direction = direction;
    layout();
  }
}

void FlexContainer::layout(){
  Component::layout();
  layoutChildren();
}

void FlexContainer::layoutChildren(){
  // 子の大きさを決める
  // このとき主軸サイズfillと固定の合計、交差軸サイズ固定の最大をとっておく
  float mainAxisTotalFillSize = 0;
  float mainAxisTotalFixedSize = 0;
  float counterAxisMaxSize = 0;
  for(auto& child : children){
    child->computeSize();
    auto childSizeAxis = getAxisSize(child->getSize());
    auto childLayoutSizeAxis = getAxisSize(child->getLayoutSize());
    if(getMainAxisUnit(child.get()) == SizeUnit::FILL_CONTAINER){
      mainAxisTotalFillSize += childSizeAxis.x;
    }else{
      mainAxisTotalFixedSize += childLayoutSizeAxis.x;
    }
    if(getCrossAxisUnit(child.get()) != SizeUnit::FILL_CONTAINER){
      counterAxisMaxSize = std::max(counterAxisMaxSize, childLayoutSizeAxis.y);
    }
  }

  // 子を配置 & fillの大きさを決める
  Math::Vector2 selfSizeAxis = getAxisSize(layoutSize);
  Math::Vector2 childPosAxis(0, 0);
  for(auto& child : children){
    auto childSizeAxis = getAxisSize(child->getSize());
    auto childLayoutSizeAxis = getAxisSize(child->getLayoutSize());
    if(getMainAxisUnit(child.get()) == SizeUnit::FILL_CONTAINER){
      childLayoutSizeAxis.x = std::max(0.0f, (childSizeAxis.x / mainAxisTotalFillSize) * (selfSizeAxis.x - mainAxisTotalFixedSize));
    }
    // xy→軸サイズの変換関数だけどたぶん逆もいける
    child->layoutPosition = getAxisSize(childPosAxis);
    child->layoutSize = getAxisSize(childLayoutSizeAxis);

    // 子の大きさが確定したので孫を配置
    if(auto childAsContainer = std::dynamic_pointer_cast<Container>(child)){
      childAsContainer->layoutChildren();
    }

    childPosAxis.x += childLayoutSizeAxis.x;
  }
}

Math::Vector2 FlexContainer::getAxisSize(const Math::Vector2 size) const{
  if(direction == Direction::VERTICAL){
    return Math::Vector2(size.y, size.x);
  }
  return size;
}

SizeUnit FlexContainer::getMainAxisUnit(Component* comp) const{
  return direction == Direction::VERTICAL ? comp->getHeightUnit() : comp->getWidthUnit();
}

SizeUnit FlexContainer::getCrossAxisUnit(Component* comp) const{
  return direction == Direction::VERTICAL ? comp->getWidthUnit() : comp->getHeightUnit();
}

}
