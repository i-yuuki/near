#include "pch.h"
#include "layer.h"

#include "scene.h"

namespace Near{

void Layer::init(Scene* scene){
  this->scene = scene;
  /*
  for(auto obj : objects){
    obj->init(this);
  }
  */
}

void Layer::beforeUpdate(float deltaTime){
  for(auto obj : objects){
    obj->beforeUpdate(deltaTime);
  }
}

void Layer::update(float deltaTime){
  for(auto obj : objects){
    obj->update(deltaTime);
  }
  auto result = std::remove_if(objects.begin(), objects.end(), [](std::shared_ptr<GameObject>& obj){ return obj->isRemoveMarked(); });
  std::for_each(result, objects.end(), [](std::shared_ptr<GameObject>& obj){ obj->uninit(); });
  objects.erase(result, objects.end());
}

void Layer::afterUpdate(float deltaTime){
  for(auto obj : objects){
    obj->afterUpdate(deltaTime);
  }
}

void Layer::draw(){
  for(auto obj : objects){
    obj->draw();
  }
}

void Layer::uninit(){
  for(auto obj : objects){
    obj->uninit();
  }
  objects.clear();
}

Scene* Layer::getScene(){
  return scene;
}

}
