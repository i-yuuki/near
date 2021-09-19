#include "pch.h"
#include "gui/container.h"

#include "near.h"

namespace Near::GUI{

void ContainerBase::draw(){
  Component::draw();
  renderer()->pushWorldTransform();
  renderer()->applyWorldTransform(Math::Matrix::CreateTranslation(position.x, position.y, 0));
  for(auto& child : children){
    child->draw();
  }
  renderer()->popWorldTransform();
}

}
