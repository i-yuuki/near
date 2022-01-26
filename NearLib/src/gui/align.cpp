#include "pch.h"
#include "gui/align.h"

namespace Near::GUI{

Align::Align(const Math::Vector2& align) : align(align){
}

void Align::layout(const BoxConstraints& constraints){
  if(!child) return;
  child->layout(constraints);
  if(auto p = parent.lock()){
    layoutPosition = (p->getLayoutSize() - child->getLayoutSize()) * align;
  }else{
    layoutPosition = Math::Vector2::Zero;
  }
  layoutSize = child->getLayoutSize();
}

std::shared_ptr<Align> Align::Create(std::shared_ptr<Component> child, const Math::Vector2& align){
  auto comp = std::make_shared<Align>(align);
  comp->setChild(child);
  return comp;
}

}
