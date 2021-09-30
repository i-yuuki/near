#include "pch.h"
#include "gui/container.h"

#include "near.h"

namespace Near::GUI{

void Container::draw(){
  Component::draw();
  renderer()->pushWorldTransform();
  renderer()->applyWorldTransform(Math::Matrix::CreateTranslation(position.x, position.y, 0));
  for(auto& child : children){
    child->draw();
  }
  renderer()->popWorldTransform();
}

void Container::add(std::shared_ptr<Component> child){
  if(!child->getParent().expired()){
    // 既にどこかに追加されていたらなんかする？
  }
  children.push_back(child);
  child->parent = this->shared_from_this();
  childAdded(child);
}

void Container::layout(){
}

void Container::sizeChanged(){
  layout();
}

void Container::childAdded(std::shared_ptr<Component> child){
  layout();
}

}
