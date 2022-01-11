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
  
  if(GetAsyncKeyState('W')){
    movement.z += speed;
  }
  if(GetAsyncKeyState('S')){
    movement.z -= speed;
  }
  if(GetAsyncKeyState('D')){
    movement.x += speed;
  }
  if(GetAsyncKeyState('A')){
    movement.x -= speed;
  }
  if(GetAsyncKeyState('E')){
    movement.y += speed;
  }
  if(GetAsyncKeyState('Q')){
    movement.y -= speed;
  }
  
  float speedMultiplier = (GetAsyncKeyState(VK_SHIFT) ? 0.25f : GetAsyncKeyState(VK_CONTROL) ? 4.0f : 1.0f);
  movement = (movement.x * right + movement.y * Near::Math::Vector3::Up + movement.z * forward) * speedMultiplier;
  move(movement, deltaSeconds);
  
  Near::Math::Vector2 rotation;

  if(GetAsyncKeyState(VK_RIGHT)){
    rotation.x += rotationSpeed;
  }
  if(GetAsyncKeyState(VK_LEFT)){
    rotation.x -= rotationSpeed;
  }
  if(GetAsyncKeyState(VK_DOWN)){
    rotation.y += rotationSpeed;
  }
  if(GetAsyncKeyState(VK_UP)){
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
