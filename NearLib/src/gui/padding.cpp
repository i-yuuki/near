#include "pch.h"
#include "gui/padding.h"

namespace Near::GUI{

Padding::Padding(float vertical, float horizontal)
  : Padding(vertical, horizontal, vertical, horizontal){
}
Padding::Padding(float top, float right, float bottom, float left)
  : top(top), right(right), bottom(bottom), left(left){
}

void Padding::layout(const BoxConstraints& constraints){
  if(!child) return;
  BoxConstraints childConstraints;
  if(std::isfinite(constraints.maxWidth)){
    // minWidth/minHeight 必要かも？
    childConstraints.maxWidth = std::max(childConstraints.minWidth, constraints.maxWidth) - left - right;
  }
  if(std::isfinite(constraints.maxHeight)){
    childConstraints.maxHeight = std::max(childConstraints.minHeight, constraints.maxHeight) - top - bottom;
  }
  child->layout(childConstraints);
  child->layoutPosition = Math::Vector2(left, top);
  layoutSize = child->getLayoutSize();
  layoutSize.x += left + right;
  layoutSize.y += top + bottom;
}

}
