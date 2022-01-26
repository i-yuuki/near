#include "pch.h"
#include "player.h"

#include <algorithm>

#include <NearLib/collision-3d.h>
#include <NearLib/scene.h>
#include <NearLib/utils.h>

#include "game.h"
#include "main.h"
#include "portal-scene.h"
#include "scene-game.h"
#include "scene-finish.h"
#include "level-object.h"

void Player::init(Near::Layer* layer){
  Near::GameObject::init(layer);
  size = Near::Math::Vector3(32, 72, 32);
  onGround = false;
  thirdPerson = false;
  keyListener = Near::input()->onKeyDown.addListener([this](Near::InputManager::KeyEvent e){
    printf_s("Player received keyDown: %d\n", e.vkey);
    if(e.vkey == VK_SPACE){
      if(!e.isRepeat && onGround){
        velocity.y = 200;
      }
    }else if(e.vkey == 'E'){
      interact();
    #ifdef _DEBUG
    }else if(e.vkey == 'F'){
      thirdPerson = !thirdPerson;
    #endif
    }
  });
}

void Player::update(float deltaTime){
  PortalTraveler::update(deltaTime);

  auto* input = Near::input();

  constexpr float sensitivity = 3.5f;
  float yaw = -input->getMouseMovementX() * 0.022f * sensitivity;
  float pitch = -input->getMouseMovementY() * 0.022f * sensitivity;
  transform.rotation = Near::Math::Quaternion::CreateFromYawPitchRoll(0, DirectX::XMConvertToRadians(pitch), 0) * transform.rotation * Near::Math::Quaternion::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(yaw), 0, 0);

  Near::Math::Vector2 targetMovement;
  if(input->isKeyDown('W')){
    targetMovement.y =  1;
  }
  if(input->isKeyDown('S')){
    targetMovement.y = -1;
  }
  if(input->isKeyDown('D')){
    targetMovement.x =  1;
  }
  if(input->isKeyDown('A')){
    targetMovement.x = -1;
  }
  targetMovement.Normalize();
  movement = Near::Math::Vector2::Lerp(movement, targetMovement, std::min(1.0f, deltaTime * 0.2f));
  
  Near::Math::Vector3 forward = transform.getForward();
  forward.y = 0;
  forward.Normalize();
  
  Near::Math::Vector3 right = transform.getRight();
  right.y = 0;
  right.Normalize();

  onGround = false;

  constexpr float walkSpeed = 200; // units per second
  auto walk = (movement.x * right + movement.y * forward) * walkSpeed;
  move(walk, deltaTime * 0.001f);

  constexpr float gravity = -500;
  velocity.y += gravity * (deltaTime * 0.001f);
  move(velocity, deltaTime * 0.001f);

  auto* game = NearGame::Game::Instance;
  if(transform.position.y < NearGame::VOID_Y){
    game->levels.setNextLevel(game->levels.getLevelIdx());
    game->fadeToNextScene<SceneGame>(Near::Math::Color(0, 0, 0, 1), 500);
    return;
  }

  float distanceToFinish = Near::Math::Vector3::DistanceSquared(transform.position, game->levels.getLevel()->getFinishPosition());
  if(distanceToFinish < 64 * 64){
    game->fadeToNextScene<SceneFinish>(NearGame::BACKGROUND_COLOR, 1000);
    return;
  }

  if(onGround){
    velocity.y = 0;
  }

  if(auto cube = cubeHolding.lock()){
    cube->transform.position = transform.position;
    cube->transform.position.y += size.y / 2;
    auto fwd = transform.getForward() * 128;
    cube->move(fwd, 1);
  }
}

void Player::draw(){
}

void Player::uninit(){
  keyListener->disconnect();
}

void Player::addColliders(std::function<void(const Near::Collision::BoundingBox3D&)> out){
  out(Near::Collision::BoundingBox3D(transform.position, size / 2));
}

bool Player::isThirdPerson() const{
  return thirdPerson;
}

void Player::interact(){
  if(auto cube = cubeHolding.lock()){
    cube->setHolder(nullptr);
    cubeHolding.reset();
    return;
  }
  // とりあえず一番近いキューブを拾う
  std::vector<std::shared_ptr<Cube>> cubes;
  getLayer()->getScene()->findObjectsOfExactType<Cube>(cubes);
  float nearest;
  std::shared_ptr<Cube> cubeToPick;
  for(auto& cube : cubes){
    float distance = Near::Math::Vector3::DistanceSquared(cube->transform.position, transform.position);
    if(distance >= 128 * 128) continue;
    if(!cubeToPick || distance < nearest){
      cubeToPick = cube;
      nearest = distance;
    }
  }
  if(cubeToPick){
    cubeToPick->setHolder(this);
    cubeHolding = cubeToPick;
  }
}
