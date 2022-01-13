#include "pch.h"
#include "gui/has-child.h"

#include "near.h"

namespace Near::GUI{

HasChild::HasChild(std::shared_ptr<Component> child){
  setChild(child);
}

void HasChild::setChild(std::shared_ptr<Component> child){
  if(child != nullptr){
    if(!child->getParent().expired()){
      // 既にどこかに追加されていたらなんかする？
    }
    child->parent = this->shared_from_this();
  }
  this->child = child;
}

void HasChild::draw(){
  Component::draw();
  renderer2D()->pushTransform();
  renderer2D()->translate(layoutPosition.x, layoutPosition.y);
  child->draw();
  renderer2D()->popTransform();
}

void HasChildren::add(std::shared_ptr<Component> child){
  // 自分のchildrenに複数回追加しない
  if(std::find(children.begin(), children.end(), child) != children.end()) return;
  
  if(child != nullptr){
    if(!child->getParent().expired()){
      // 既にどこかに追加されていたらなんかする？
    }
    child->parent = this->shared_from_this();
  }
  children.push_back(child);
}

void HasChildren::draw(){
  Component::draw();
  renderer2D()->pushTransform();
  renderer2D()->translate(layoutPosition.x, layoutPosition.y);
  for(auto& child : children){
    child->draw();
  }
  renderer2D()->popTransform();
}

}
