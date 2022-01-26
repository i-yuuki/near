#include "pch.h"
#include "gui/container.h"

#include "near.h"

namespace Near::GUI{

const std::optional<Length>& Container::getWidth() const{
  return width;
}

const std::optional<Length>& Container::getHeight() const{
  return height;
}

void Container::setWidth(const std::optional<Length>& width){
  this->width = width;
}

void Container::setHeight(const std::optional<Length>& height){
  this->height = height;
}

void Container::layout(const BoxConstraints& constraints){
  bool hasMaxWidth = std::isfinite(constraints.maxWidth);
  bool hasMaxHeight = std::isfinite(constraints.maxHeight);
  // 自分のサイズを決められそうなら決める
  BoxConstraints childConstraints;
  if(width.has_value()){
    switch(width->unit){
      case Unit::PX:      layoutSize.x = width->value; break;
      case Unit::PERCENT: layoutSize.x = hasMaxWidth ? constraints.maxWidth * width->value / 100 : 1; break;
      default: throw std::exception("Unknown width unit! (missing implementation?)");
    }
    layoutSize.x = std::clamp(layoutSize.x, constraints.minWidth, constraints.maxWidth);
    childConstraints.setMaxWidth(layoutSize.x);
  }
  if(height.has_value()){
    switch(height->unit){
      case Unit::PX:      layoutSize.y = height->value; break;
      case Unit::PERCENT: layoutSize.y = hasMaxHeight ? constraints.maxHeight * height->value / 100 : 1; break;
      default: throw std::exception("Unknown height unit! (missing implementation?)");
    }
    layoutSize.y = std::clamp(layoutSize.y, constraints.minHeight, constraints.maxHeight);
    childConstraints.setMaxHeight(layoutSize.y);
  }

  Near::Math::Vector2 largestChildSize;
  for(auto& child : children){
    child->layout(childConstraints);
    largestChildSize = Near::Math::Vector2::Max(largestChildSize, child->layoutSize);
  }
  
  // 自動サイズ
  if(!width.has_value()){
    // 三項演算子黒魔法！ｗ (よい子はマネしないでね)
    layoutSize.x = std::clamp(children.empty() ? hasMaxWidth ? constraints.maxWidth : 1 : largestChildSize.x, constraints.minWidth, constraints.maxWidth);
  }
  if(!height.has_value()){
    layoutSize.y = std::clamp(children.empty() ? hasMaxHeight ? constraints.maxHeight : 1 : largestChildSize.y, constraints.minHeight, constraints.maxHeight);
  }
}

}
