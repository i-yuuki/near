#include "portal-traveler.h"

void PortalTraveler::update(float deltaTime){
  lastPosition = transform.position;
  GameObject::update(deltaTime);
}
