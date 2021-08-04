#include "portal-camera.h"

void PortalCamera::update(float deltaTime){
  PortalTraveler::update(deltaTime);
  Near::DebugCamera::update(deltaTime);
}
