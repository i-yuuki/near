#include "activatable.h"

bool Activatable::isActive(){
  return active;
}

void Activatable::setActive(bool active){
  if(this->active == active) return;
  this->active = active;
  if(active){
    activate();
  }else{
    deactivate();
  }
}

void Activator::addTarget(std::weak_ptr<Activatable> target){
  targets.push_back(target);
}

void Activator::activateTargets(){
  for(auto& target : targets){
    if(auto targetObj = target.lock()){
      targetObj->setActive(true);
    }
  }
}

void Activator::deactivateTargets(){
  for(auto& target : targets){
    if(auto targetObj = target.lock()){
      targetObj->setActive(false);
    }
  }
}
