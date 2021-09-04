#include "floor-button.h"

#include <NearLib/scene.h>

FloorButton::FloorButton() : GameObject(){
}

void FloorButton::init(Near::Layer* layer){
  GameObject::init(layer);
  model.reset(new Near::FBXModel());
  model->load("assets/models/floor-button.fbx");
  vertexShader = Near::Assets::vertexShaders()->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = Near::Assets::pixelShaders()->getOrLoad("assets/shaders/ps-level.hlsl");
  active = false;
}

void FloorButton::draw(){
  auto* r = Near::renderer();
  auto t = transform.createTransform();
  r->setVertexShader(vertexShader.get());
  r->setPixelShader(pixelShader.get());
  model->draw(&t);
}

void FloorButton::uninit(){
  vertexShader.reset();
  pixelShader.reset();
  model.reset();
  GameObject::uninit();
}

void FloorButton::activate(){
  activateTargets();
}

void FloorButton::deactivate(){
  deactivateTargets();
}
