#include "door.h"

#include <algorithm>

#include <NearLib/scene.h>

Door::Door() : GameObject(){
}

void Door::init(Near::Layer* layer){
  GameObject::init(layer);
  time = 0;
  active = false;
  model.reset(new Near::FBXModel());
  model->load("assets/models/door.fbx");
}

void Door::update(float deltaTime){
  static constexpr float duration = 45.0f / 60.0f;
  if(active){
    time = std::min(time + deltaTime / 1000, duration);
  }else{
    time = std::max(time - deltaTime / 1000, 0.0f);
  }
}

void Door::draw(){
  auto t = transform.createTransform();
  model->setAnimation(0, time);
  model->draw(&t);
}

void Door::uninit(){
  GameObject::uninit();
  model.reset();
}

void Door::activate(){
}

void Door::deactivate(){
}
