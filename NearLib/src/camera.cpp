#include "pch.h"
#include "camera.h"

#include "near.h"

namespace Near{

float Camera::getFOV(){
  return fov;
}

void Camera::setFOV(float degrees){
  fov = degrees;
}

float Camera::getNear(){
  return nearPlane;
}

void Camera::setNear(float nearPlane){
  this->nearPlane = nearPlane;
}

float Camera::getFar(){
  return farPlane;
}

void Camera::setFar(float farPlane){
  this->farPlane = farPlane;
}

void Camera::draw(){
  auto* r = renderer();
  r->setViewTransform(createViewTransform());
  r->setProjectionTransform(createProjectionTransform());
}

Math::Matrix Camera::createViewTransform(){
  return Math::Matrix::CreateLookAt(transform.position, transform.position + transform.getForward(), Math::Vector3::Up);
}

Math::Matrix Camera::createProjectionTransform(){
  auto* r = renderer();
  float aspect = static_cast<float>(r->getWidth()) / r->getHeight();
  float radians = DirectX::XMConvertToRadians(fov / 2);
  return Math::Matrix::CreatePerspectiveFieldOfView(radians, aspect, nearPlane, farPlane);
}

}
