#include "pch.h"
#include "game-object.h"

namespace Near{

GameObject::GameObject(){
}

void GameObject::init(Layer* layer){
  this->layer = layer;
}

void GameObject::beforeUpdate(float deltaTime){
}

void GameObject::update(float deltaTime){
}

void GameObject::afterUpdate(float deltaTime){
}

void GameObject::draw(){
}

void GameObject::uninit(){
}

Layer* GameObject::getLayer(){
  return layer;
}

bool GameObject::isRemoveMarked(){
  return removeMarked;
}

void GameObject::markRemove(){
  removeMarked = true;
}

}
