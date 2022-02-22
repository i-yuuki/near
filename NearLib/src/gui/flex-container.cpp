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

FlexContainer::CrossAxisAlign FlexContainer::getCrossAxisAlign() const{
  return crossAxisAlign;
}

float FlexContainer::getGap() const{
  return gap;
}

void FlexContainer::setDirection(Direction direction){
  if(direction != this->direction){
    this->direction = direction;
    // layout();
  }
}

void FlexContainer::setCrossAxisAlign(CrossAxisAlign align){
  this->crossAxisAlign = align;
}

void FlexContainer::setGap(float gap){
  this->gap = gap;
}

void FlexContainer::layout(const BoxConstraints& constraints){
  // CSSの仕様とFlutterのソースを参考
  // https://api.flutter.dev/flutter/widgets/Flex-class.html

  Near::Math::Vector2 totalSizeAxis;

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
      if(crossAxisAlign == CrossAxisAlign::STRETCH){
        switch(direction){
          case Direction::HORIZONTAL: childConstraints.setHeight(constraints.maxHeight); break;
          case Direction::VERTICAL:   childConstraints.setWidth(constraints.maxWidth); break;
          default: throw std::exception("Unknown FlexContainer direction! (missing implementation?)");
        }
      }else{
        switch(direction){
          case Direction::HORIZONTAL: childConstraints.setMaxHeight(constraints.maxHeight); break;
          case Direction::VERTICAL:   childConstraints.setMaxWidth(constraints.maxWidth); break;
          default: throw std::exception("Unknown FlexContainer direction! (missing implementation?)");
        }
      }
      child->layout(childConstraints);
      mainAxisTotalFixedSize += getAxisSize(child->layoutSize).x;
    }
  }

  // 2. 主軸サイズflexの子で残りの主軸スペースを分け合う
  // 3. 子を配置
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

    totalSizeAxis.x += childLayoutSizeAxis.x;
    totalSizeAxis.y = std::max(totalSizeAxis.y, childLayoutSizeAxis.y);
  }
  
  // 交差軸配置
  float crossAxisAlignVal = 0;
  switch(crossAxisAlign){
    case CrossAxisAlign::START:   crossAxisAlignVal = 0; break;
    case CrossAxisAlign::CENTER:  crossAxisAlignVal = 0.5f; break;
    case CrossAxisAlign::END:     crossAxisAlignVal = 1; break;
    case CrossAxisAlign::STRETCH: crossAxisAlignVal = 0; break;
    default: throw std::exception("Unknown FlexContainer crossAxisAlign! (missing implementation?)");
  }
  for(auto& child : children){
    auto childLayoutPosAxis = getAxisSize(child->layoutPosition);
    auto childLayoutSizeAxis = getAxisSize(child->getLayoutSize());
    childLayoutPosAxis.y += (totalSizeAxis.y - childLayoutSizeAxis.y) * crossAxisAlignVal;
    child->layoutPosition = getAxisSize(childLayoutPosAxis);
  }

  totalSizeAxis.x += std::max(size_t{0}, children.size() - 1) * gap;
  layoutSize = getAxisSize(totalSizeAxis);
}

Math::Vector2 FlexContainer::getAxisSize(const Math::Vector2 size) const{
  if(direction == Direction::VERTICAL){
    return Math::Vector2(size.y, size.x);
  }
  return size;
}

}
