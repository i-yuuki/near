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
  // 1. 主軸サイズautoと固定の合計を求め、のこりを割ってgrowに使う
  // 2. 交差軸軸サイズautoと固定の最大を求め、growに使う
  // 3. 完成
  float mainAxisTotalSize = 0;
  float counterAxisMaxSize = 0;
  for(auto& child : children){
    auto childSizeAxis = getAxisSize(child->getSize());
    mainAxisTotalSize += childSizeAxis.x;
    counterAxisMaxSize = std::max(counterAxisMaxSize, childSizeAxis.y);
  }

  Math::Vector2 selfSizeAxis = getAxisSize(size);
  float pos = 0;
  for(auto& child : children){
    Math::Vector2 childPosAxis(pos, 0);
    auto childSizeAxis = getAxisSize(child->getSize());
    auto childLayoutSizeAxis = childSizeAxis;
    SizeUnit childCrossUnit = getCrossAxisUnit(child.get());
    if(childCrossUnit == SizeUnit::FILL_CONTAINER){
      childLayoutSizeAxis.y = counterAxisMaxSize;
    }else if(childCrossUnit == SizeUnit::PARENT){
      childLayoutSizeAxis.y = selfSizeAxis.y * childSizeAxis.y;
    }
    child->layoutPosition = getAxisSize(childPosAxis);
    child->layoutSize = getAxisSize(childLayoutSizeAxis);
    pos += childSizeAxis.x;
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
