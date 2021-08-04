#include "pch.h"
#include "transform.h"

namespace Near{

Math::Vector3 Transform::getForward(){
  Math::Matrix rot = Math::Matrix::CreateFromQuaternion(rotation);
  return rot.Forward();
}

Math::Vector3 Transform::getRight(){
  Math::Matrix rot = Math::Matrix::CreateFromQuaternion(rotation);
  return rot.Right();
}

Math::Vector3 Transform::getUp(){
  Math::Matrix rot = Math::Matrix::CreateFromQuaternion(rotation);
  return rot.Up();
}

Math::Matrix Transform::createTransform(){
  return Math::Matrix::CreateScale(scale) * Math::Matrix::CreateFromQuaternion(rotation) * Math::Matrix::CreateTranslation(position);
}

}
