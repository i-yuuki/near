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
  vertexShader = layer->getScene()->vertexShaders->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = layer->getScene()->pixelShaders->getOrLoad("assets/shaders/ps-level.hlsl");
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
  auto* r = Near::renderer();
  auto t = transform.createTransform();
  r->setVertexShader(vertexShader.get());
  r->setPixelShader(pixelShader.get());
  model->setAnimation(0, time);
  model->draw(&t);
}

void Door::uninit(){
  vertexShader.reset();
  pixelShader.reset();
  model.reset();
  GameObject::uninit();
}

void Door::addColliders(std::function<void(const Near::Collision::BoundingBox3D&)> out){
  if(!active){
    Near::Math::Vector3 points[] = {
      Near::Math::Vector3(-64, 128,  0),
      Near::Math::Vector3( 64, 128,  0),
      Near::Math::Vector3(-64,   0,  0),
      Near::Math::Vector3( 64,   0,  0),
      Near::Math::Vector3(-64, 128, -6),
      Near::Math::Vector3( 64, 128, -6),
      Near::Math::Vector3(-64,   0, -6),
      Near::Math::Vector3( 64,   0, -6),
    };
    auto t = transform.createTransform();
    for(auto& point : points){
      point = Near::Math::Vector3::Transform(point, t);
    }
    out(Near::Collision::BoundingBox3D::FromPoints(points, 8));
  }
}

void Door::activate(){
}

void Door::deactivate(){
}
