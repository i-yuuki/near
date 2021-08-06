#include "player-camera.h"

#include <NearLib/scene.h>

#include "player.h"

PlayerCamera::PlayerCamera(std::weak_ptr<Player> player) : player(player){
}

void PlayerCamera::afterUpdate(float deltaTime){
  if(auto p = player.lock()){
    transform = p->transform;
    transform.position.y += p->getSize().y / 2;
    if(p->isThirdPerson()){
      transform.position -= p->transform.getForward() * 150;
    }
  }else{
    markRemove();
    return;
  }
}
