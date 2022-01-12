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

  {
    auto result = std::remove_if(objects.begin(), objects.end(), [](std::shared_ptr<GameObject>& obj){
      if(obj->isRemoveMarked()){
        obj->uninit();
        return true;
      }
      return false;
    });
    objects.erase(result, objects.end());
  }

  {
    auto result = std::remove_if(collidables.begin(), collidables.end(), [](auto& obj){ return obj.expired(); });
    collidables.erase(result, collidables.end());
  }
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

void Layer::findColliders(std::function<void(std::shared_ptr<GameObject>, const Collision::BoundingBox3D&)> callback){
  for(auto& c : collidables){
    auto obj = c.lock();
    if(!obj) continue;
    std::dynamic_pointer_cast<ICollidable>(obj)->addColliders([&](auto aabb){
      callback(obj, aabb);
    });
  }
}

Scene* Layer::getScene(){
  return scene;
}

}
