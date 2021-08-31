#include "floor-button.h"

#include <NearLib/scene.h>

FloorButton::FloorButton() : GameObject(){
}

void FloorButton::init(Near::Layer* layer){
  GameObject::init(layer);
  model.reset(new Near::FBXModel());
  model->load("assets/models/floor-button.fbx");
  active = false;
}

void FloorButton::draw(){
  auto t = transform.createTransform();
  model->draw(&t);
}

void FloorButton::uninit(){
  GameObject::uninit();
  model.reset();
}

void FloorButton::activate(){
  activateTargets();
}

void FloorButton::deactivate(){
  deactivateTargets();
}
