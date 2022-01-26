#include "pch.h"
#include "cube.h"

#include <NearLib/scene.h>
#include <NearLib/utils.h>

#include "game.h"
#include "main.h"
#include "player.h"
#include "scene-game.h"

Cube::Cube() : PortalTraveler(){
}

void Cube::init(Near::Layer* layer){
  PortalTraveler::init(layer);
  size = Near::Math::Vector3(32, 32, 32);
  model.reset(new Near::FBXModel());
  model->load("assets/models/cube.fbx");
  vertexShader = Near::Assets::vertexShaders()->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = Near::Assets::pixelShaders()->getOrLoad("assets/shaders/ps-level.hlsl");
}

void Cube::update(float deltaTime){
  if(holder){
    velocity = Near::Math::Vector3::Zero;
    return;
  }
  constexpr float gravity = -500;
  velocity.y += gravity * (deltaTime * 0.001f);
  velocity.x -= std::abs(velocity.x) * (deltaTime * 0.001f) * Near::sign(velocity.x);
  velocity.z -= std::abs(velocity.z) * (deltaTime * 0.001f) * Near::sign(velocity.z);
  move(velocity, deltaTime * 0.001f);

  if(transform.position.y < NearGame::VOID_Y){
    auto* game = NearGame::Game::Instance;
    game->levels.setNextLevel(game->levels.getLevelIdx());
    game->fadeToNextScene<SceneGame>(Near::Math::Color(0, 0, 0, 1), 500);
    return;
  }

  if(button.expired()){
    std::vector<std::shared_ptr<FloorButton>> buttons;
    getLayer()->getScene()->findObjectsOfExactType<FloorButton>(buttons);
    float nearest;
    std::shared_ptr<FloorButton> buttonToPress;
    for(auto& button : buttons){
      float distance = Near::Math::Vector3::DistanceSquared(button->transform.position, transform.position);
      if(distance >= 32 * 32) continue;
      if(!buttonToPress || distance < nearest){
        buttonToPress = button;
        nearest = distance;
      }
    }
    if(buttonToPress && !buttonToPress->isActive()){
      this->button = buttonToPress;
      buttonToPress->setActive(true);
      transform.position = buttonToPress->transform.position;
      transform.position.y += size.y / 2;
    }
  }
}

void Cube::draw(){
  auto* r = Near::renderer();
  auto t = transform.createTransform();
  r->setVertexShader(vertexShader.get());
  r->setPixelShader(pixelShader.get());
  model->draw(&t);
}

void Cube::uninit(){
  vertexShader.reset();
  pixelShader.reset();
  model.reset();
}

void Cube::addColliders(std::function<void(const Near::Collision::BoundingBox3D&)> out){
  out(Near::Collision::BoundingBox3D(transform.position, size / 2));
}

void Cube::setHolder(Player* holder){
  if(holder == this->holder) return;
  this->holder = holder;
  if(holder){
    if(auto btn = button.lock()){
      btn->setActive(false);
      button.reset();
    }
  }
}
