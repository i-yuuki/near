#pragma once

#include "near.h"
#include "game-object.h"
#include "collidable.h"

namespace Near{

class Scene;

class Layer{
public:
  void init(Scene* scene);
  void beforeUpdate(float deltaTime);
  void update(float deltaTime);
  void afterUpdate(float deltaTime);
  void draw();
  void uninit();
  void findColliders(std::function<void(std::shared_ptr<GameObject>, const Collision::BoundingBox3D&)> callback);
  Scene* getScene();
  template<class T>
  std::shared_ptr<T> findObjectOfType(){
    const std::type_info& t = typeid(T);
    for(auto obj : objects){
      if(auto ret = std::dynamic_pointer_cast<T>(obj)) return ret;
    }
    return nullptr;
  }
  template<class T>
  std::shared_ptr<T> findObjectOfExactType(){
    const std::type_info& t = typeid(T);
    for(auto obj : objects){
      if(typeid(*obj) == t) return std::dynamic_pointer_cast<T>(obj);
    }
    return nullptr;
  }
  template<class T>
  int findObjectsOfType(std::vector<std::shared_ptr<T>>& out){
    const std::type_info& t = typeid(T);
    int total = 0;
    for(auto obj : objects){
      if(auto ret = std::dynamic_pointer_cast<T>(obj)){
        out.push_back(ret);
        total ++;
      }
    }
    return total;
  }
  template<class T>
  int findObjectsOfExactType(std::vector<std::shared_ptr<T>>& out){
    const std::type_info& t = typeid(T);
    int total = 0;
    for(auto obj : objects){
      if(typeid(*obj) == t){
        out.push_back(std::dynamic_pointer_cast<T>(obj));
        total ++;
      }
    }
    return total;
  }
  template<class T, typename... Args>
  std::shared_ptr<T> createGameObject(Args&&... args){
    auto obj = std::make_shared<T>(std::forward<Args>(args)...);
    obj->init(this);
    objects.push_back(obj);
    if(std::is_base_of<ICollidable, T>::value){
      collidables.push_back(std::weak_ptr<GameObject>(obj));
    }
    return obj;
  }
private:
  Scene* scene = nullptr;
  std::vector<std::shared_ptr<GameObject>> objects;
  std::vector<std::weak_ptr<GameObject>> collidables;
};

}
