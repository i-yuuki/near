#include "pch.h"
#include "portal-camera.h"

void PortalCamera::update(float deltaTime){
  PortalTraveler::update(deltaTime);

  if(!debugControls) return;

  // DebugCameraをうまく使えるといいけど

  float deltaSeconds = deltaTime / 1000.0f;

  Near::Math::Vector3 forward = transform.getForward();
  forward.y = 0;
  forward.Normalize();
  
  Near::Math::Vector3 right = transform.getRight();
  right.y = 0;
  right.Normalize();

  Near::Math::Vector3 movement;
  
  if(Near::input()->isKeyDown('W')){
    movement.z += speed;
  }
  if(Near::input()->isKeyDown('S')){
    movement.z -= speed;
  }
  if(Near::input()->isKeyDown('D')){
    movement.x += speed;
  }
  if(Near::input()->isKeyDown('A')){
    movement.x -= speed;
  }
  if(Near::input()->isKeyDown('E')){
    movement.y += speed;
  }
  if(Near::input()->isKeyDown('Q')){
    movement.y -= speed;
  }
  
  float speedMultiplier = (Near::input()->isKeyDown(VK_SHIFT) ? 0.25f : Near::input()->isKeyDown(VK_CONTROL) ? 4.0f : 1.0f);
  movement = (movement.x * right + movement.y * Near::Math::Vector3::Up + movement.z * forward) * speedMultiplier;
  move(movement, deltaSeconds);
  
  Near::Math::Vector2 rotation;

  if(Near::input()->isKeyDown(VK_RIGHT)){
    rotation.x += rotationSpeed;
  }
  if(Near::input()->isKeyDown(VK_LEFT)){
    rotation.x -= rotationSpeed;
  }
  if(Near::input()->isKeyDown(VK_DOWN)){
    rotation.y += rotationSpeed;
  }
  if(Near::input()->isKeyDown(VK_UP)){
    rotation.y -= rotationSpeed;
  }
  rotation *= deltaSeconds;
  Near::Math::Quaternion worldRotation = Near::Math::Quaternion::CreateFromYawPitchRoll(0, -rotation.y, 0);
  Near::Math::Quaternion localRotation = Near::Math::Quaternion::CreateFromYawPitchRoll(-rotation.x, 0, 0);
  transform.rotation = worldRotation * transform.rotation * localRotation;
}

bool PortalCamera::isDebugControlsEnabled(){
  return debugControls;
}

void PortalCamera::setDebugControlsEnabled(bool enable){
  debugControls = enable;
}
