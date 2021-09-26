#include "pch.h"
#include "gui/flex-container.h"

#include "near.h"

#define MAIN_AXIS(size) (direction == Direction::HORIZONTAL ? size.x : size.y)
#define COUNTER_AXIS(size) (direction == Direction::HORIZONTAL ? size.y : size.x)
#define MAKE_VEC2(main, counter) (direction == Direction::HORIZONTAL ? Math::Vector2(main, counter) : Math::Vector2(counter, main))

namespace Near::GUI{

FlexContainer::FlexContainer(Direction direction) : direction(direction){
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

  float pos = 0;
  for(auto& child : children){
    Math::Vector2 childPosAxis(pos, 0);
    auto childSizeAxis = getAxisSize(child->getSize());
    childSizeAxis.y = counterAxisMaxSize;
    child->setPosition(getAxisSize(childPosAxis));
    child->setSize(getAxisSize(childSizeAxis));
    pos += childSizeAxis.x;
  }
}

Math::Vector2 FlexContainer::getAxisSize(const Math::Vector2 size){
  if(direction == Direction::VERTICAL){
    return Math::Vector2(size.y, size.x);
  }
  return size;
}

}
