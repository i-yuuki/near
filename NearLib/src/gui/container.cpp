#include "pch.h"
#include "gui/container.h"

#include "near.h"

namespace Near::GUI{

void Container::layout(const BoxConstraints& constraints){
  layoutSize = Near::Math::Vector2(constraints.minWidth, constraints.minHeight);
  for(auto child : children){
    child->layout(constraints);
    layoutSize.x = std::max(layoutSize.x, child->layoutSize.x);
    layoutSize.y = std::max(layoutSize.y, child->layoutSize.y);
  }
}

}
