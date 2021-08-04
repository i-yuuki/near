#include "pch.h"
#include "debug-camera.h"

#include "near.h"

namespace Near{

void DebugCamera::update(float deltaTime){
  // TODO use deltaTime
  
  Math::Vector3 forward = transform.getForward();
  forward.y = 0;
  forward.Normalize();
  
  Math::Vector3 right = transform.getRight();
  right.y = 0;
  right.Normalize();
  
  if(GetAsyncKeyState('W')){
    velocity.z += VELOCITY_ACCELERATION;
  }
  if(GetAsyncKeyState('S')){
    velocity.z -= VELOCITY_ACCELERATION;
  }
  if(GetAsyncKeyState('D')){
    velocity.x += VELOCITY_ACCELERATION;
  }
  if(GetAsyncKeyState('A')){
    velocity.x -= VELOCITY_ACCELERATION;
  }
  if(GetAsyncKeyState('E')){
    velocity.y += VELOCITY_ACCELERATION;
  }
  if(GetAsyncKeyState('Q')){
    velocity.y -= VELOCITY_ACCELERATION;
  }
  
  if(velocity.LengthSquared() > MAX_VELOCITY * MAX_VELOCITY){
    velocity.Normalize();
    velocity *= MAX_VELOCITY;
  }
  float speed = (GetAsyncKeyState(VK_SHIFT) ? 0.3f : GetAsyncKeyState(VK_CONTROL) ? 30.0f : 1.0f);
  transform.position += (velocity.x * right + velocity.z * forward + velocity.y * Math::Vector3::Up) * speed;
  velocity *= VELOCITY_ATTENUATION;
  
  if(GetAsyncKeyState(VK_RIGHT)){
    rotationVelocity.x += ROTATION_VELOCITY_ACCELERATION;
  }
  if(GetAsyncKeyState(VK_LEFT)){
    rotationVelocity.x -= ROTATION_VELOCITY_ACCELERATION;
  }
  if(GetAsyncKeyState(VK_DOWN)){
    rotationVelocity.y += ROTATION_VELOCITY_ACCELERATION;
  }
  if(GetAsyncKeyState(VK_UP)){
    rotationVelocity.y -= ROTATION_VELOCITY_ACCELERATION;
  }
  Math::Quaternion worldRotation = Math::Quaternion::CreateFromYawPitchRoll(0, -rotationVelocity.y * speed, 0);
  Math::Quaternion localRotation = Math::Quaternion::CreateFromYawPitchRoll(-rotationVelocity.x * speed, 0, 0);
  transform.rotation = worldRotation * transform.rotation * localRotation;
  rotationVelocity *= ROTATION_VELOCITY_ATTENUATION;
}

}
