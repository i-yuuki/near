#include "cube.h"

#include <NearLib/scene.h>
#include <NearLib/utils.h>

#include "player.h"

Cube::Cube() : PortalTraveler(){
}

void Cube::init(Near::Layer* layer){
  PortalTraveler::init(layer);
  size = Near::Math::Vector3(32, 32, 32);
  model.reset(new Near::FBXModel());
  model->load("assets/models/cube.fbx");
}

void Cube::update(float deltaTime){
  if(!holder.expired()){
    velocity = Near::Math::Vector3::Zero;
    return;
  }
  constexpr float gravity = -500;
  velocity.y += gravity * (deltaTime * 0.001f);
  velocity.x -= std::abs(velocity.x) * (deltaTime * 0.001f) * Near::sign(velocity.x);
  velocity.z -= std::abs(velocity.z) * (deltaTime * 0.001f) * Near::sign(velocity.z);
  move(velocity, deltaTime * 0.001f);
}

void Cube::draw(){
  auto t = transform.createTransform();
  model->draw(&t);
}

void Cube::uninit(){
  model.reset();
}
