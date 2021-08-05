#include "player.h"

#include <algorithm>

#include <NearLib/collision-3d.h>
#include <NearLib/scene.h>
#include <NearLib/utils.h>

#include "level-object.h"

void Player::init(Near::Layer* layer){
  Near::GameObject::init(layer);
  size = Near::Math::Vector3(32, 72, 32);
  onGround = false;
  keyListener = Near::input()->addKeyListener(this);
  model = new Near::FBXModel();
  model->load("assets/models/player.fbx");
  vertexShader = layer->getScene()->vertexShaders->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = layer->getScene()->pixelShaders->getOrLoad("assets/nearlib/shaders/ps.hlsl");
}

void Player::update(float deltaTime){
  PortalTraveler::update(deltaTime);

  auto* input = Near::input();
  auto levelObj = getLayer()->getScene()->findObjectOfExactType<LevelObject>();
  auto* level = levelObj ? levelObj->getLevel() : nullptr;

  constexpr float sensitivity = 3.5f;
  yaw -= input->getMouseMovementX() * 0.022f * sensitivity;
  pitch -= input->getMouseMovementY() * 0.022f * sensitivity;
  transform.rotation = Near::Math::Quaternion::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(yaw), DirectX::XMConvertToRadians(pitch), 0);

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

  constexpr float walkSpeed = 150; // units per second
  move((movement.x * right + movement.y * forward) * walkSpeed * deltaTime * 0.001f, level);

  constexpr float gravity = -500;
  velocity.y += gravity * (deltaTime * 0.001f);
  move(velocity * deltaTime * 0.001f, level);

  if(onGround){
    velocity.y = 0;
  }
}

void Player::draw(){
  auto* r = Near::renderer();
  r->setVertexShader(vertexShader.get());
  r->setPixelShader(pixelShader.get());
  Near::Math::Matrix t = Near::Math::Matrix::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(yaw), 0, 0) * Near::Math::Matrix::CreateTranslation(transform.position);
  model->draw(&t);
}

void Player::uninit(){
  Near::safeDelete(model);
  vertexShader.reset();
  pixelShader.reset();
  keyListener.disconnect();
}

void Player::onKeyDown(int vkey, bool isRepeat){
  printf_s("Player received keyDown: %d\n", vkey);
  if(vkey == VK_SPACE && !isRepeat){
    velocity.y = 200;
  }
}

const Near::Math::Vector3& Player::getSize() const{
  return size;
}

void Player::move(Near::Math::Vector3 vel, Level* level){
  if(vel == Near::Math::Vector3::Zero) return;
  Near::Collision::BoundingBox3D pBox(transform.position, size / 2);
  for(auto& block : level->getBlocks()){
    auto blockBox = Near::Collision::BoundingBox3D(block.position, block.size / 2);
    Near::Math::Vector3 hitDir;
    float hitNear;
    if(pBox.collides(vel, blockBox, nullptr, &hitDir, &hitNear)){
      // DirectXTKになくてもabsやっちゃうぞ～～
      vel += hitDir * Near::Math::Vector3(DirectX::XMVectorAbs(DirectX::XMLoadFloat3(&vel))) * (1 - hitNear);
      if(hitDir.y >= 0.5f){
        onGround = true;
      }
    }
  }
  transform.position += vel;
}
