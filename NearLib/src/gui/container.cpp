#include "pch.h"
#include "gui/container.h"

#include "near.h"

namespace Near::GUI{

void Container::draw(){
  Component::draw();
  renderer2D()->pushTransform();
  renderer2D()->translate(layoutPosition.x, layoutPosition.y);
  for(auto& child : children){
    child->draw();
  }
  renderer2D()->popTransform();
}

void Container::add(std::shared_ptr<Component> child){
  if(!child->getParent().expired()){
    // 既にどこかに追加されていたらなんかする？
  }
  children.push_back(child);
  child->parent = this->shared_from_this();
}

void Container::layout(const BoxConstraints& constraints){
  Component::layout(constraints);
  layoutChildren(constraints);
}

void Container::layoutChildren(const BoxConstraints& constraints){
  for(auto child : children){
    child->layout(constraints);
  }
}

}
