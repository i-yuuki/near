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
