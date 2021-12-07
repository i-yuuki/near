#include "pch.h"
#include "gui/flex-container.h"
#include "gui/flexible.h"

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
    // layout();
  }
}

float FlexContainer::getGap() const{
  return gap;
}

void FlexContainer::setGap(float gap){
  this->gap = gap;
}

void FlexContainer::layout(const BoxConstraints& constraints){
  Component::layout(constraints);
  layoutChildren(BoxConstraints(Math::Vector2::Zero, layoutSize));
}

void FlexContainer::layoutChildren(const BoxConstraints& constraints){
  // CSSの仕様とFlutterのソースを参考
  // https://api.flutter.dev/flutter/widgets/Flex-class.html

  // 1. 主軸サイズをflexと固定それぞれで合計しておく
  //    flex値がない子はレイアウトもしちゃう
  float mainAxisTotalFlex = 0;
  float mainAxisTotalFixedSize = 0;
  float mainAxisMaxSelfSize = direction == Direction::HORIZONTAL ? constraints.maxWidth : constraints.maxHeight;
  bool canFlex = std::isfinite(mainAxisMaxSelfSize);
  for(auto& child : children){
    if(auto flexible = std::dynamic_pointer_cast<Flexible>(child)){
      mainAxisTotalFlex += flexible->getFlex();
    }else{
      BoxConstraints childConstraints;
      switch(direction){
        case Direction::HORIZONTAL: childConstraints.setMaxHeight(constraints.maxHeight); break;
        case Direction::VERTICAL:   childConstraints.setMaxWidth(constraints.maxWidth); break;
        default: throw std::exception("Unknown FlexContainer direction! (missing implementation?)");
      }
      child->layout(childConstraints);
      mainAxisTotalFixedSize += getAxisSize(child->layoutSize).x;
    }
  }

  // 2. 主軸サイズflexの子で残りの主軸スペースを分け合う
  // 3. 子を配置
  Math::Vector2 selfSizeAxis = getAxisSize(layoutSize);
  Math::Vector2 childPosAxis(0, 0);
  float totalGap = (children.size() - 1) * gap;
  float availableSpaceForFlex = mainAxisMaxSelfSize - mainAxisTotalFixedSize - totalGap;
  for(auto& child : children){
    if(auto flexible = std::dynamic_pointer_cast<Flexible>(child)){
      BoxConstraints childConstraints;
      if(canFlex){
        float flexSize = (flexible->getFlex() / mainAxisTotalFlex) * availableSpaceForFlex;
        switch(direction){
          case Direction::HORIZONTAL: childConstraints.setWidth(flexSize).setHeight(layoutSize.y); break;
          case Direction::VERTICAL:   childConstraints.setWidth(layoutSize.x).setHeight(flexSize); break;
          default: throw std::exception("Unknown FlexContainer direction! (missing implementation?)");
        }
      }
      child->layout(childConstraints);
    }else{
      // 主軸サイズ固定はもうレイアウト済み、ここでやることは特にない？
    }
    
    auto childLayoutSizeAxis = getAxisSize(child->getLayoutSize());
    // xy→軸サイズの変換関数だけどたぶん逆もいける
    child->layoutPosition = getAxisSize(childPosAxis);

    childPosAxis.x += childLayoutSizeAxis.x;
    childPosAxis.x += gap;
  }
}

Math::Vector2 FlexContainer::getAxisSize(const Math::Vector2 size) const{
  if(direction == Direction::VERTICAL){
    return Math::Vector2(size.y, size.x);
  }
  return size;
}

}
