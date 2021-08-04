#include "player-camera.h"

#include <NearLib/scene.h>

#include "player.h"

PlayerCamera::PlayerCamera(std::weak_ptr<Player> player) : player(player){
}

void PlayerCamera::afterUpdate(float deltaTime){
  if(Near::input()->isKeyPressedThisFrame('F')){
    isThirdPerson = !isThirdPerson;
  }
  if(auto p = player.lock()){
    transform = p->transform;
    // transform.position += offset.x * p->transform.getRight() * offset.y * p->transform.getUp() * offset.z * p->transform.getForward();
    transform.position.y += p->getSize().y - 0.1f;
    if(isThirdPerson){
      transform.position -= p->transform.getForward() * 3;
    }
  }else{
    markRemove();
    return;
  }
}
