#include "scene-title.h"

#include <algorithm>

#include <NearLib/utils.h>

#include "game.h"
#include "main.h"
#include "level-object.h"
#include "polygon-2d.h"
#include "scene-game.h"
#include "scene-guitest.h"

SceneTitle::SceneTitle() : PortalScene(){
}

void SceneTitle::init(){
  PortalScene::init();
  auto* r = Near::renderer();
  level.reset(new Level());
  level->load("assets/levels/title.txt");
  getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<LevelObject>(level);
  level->createGameObjects(*this);
  camera = getLayer(Near::Scene::LAYER_MANAGERS)->createGameObject<PortalCamera>();
  camera->setFar(8000);
  title = getLayer(Near::Scene::LAYER_OVERLAY)->createGameObject<Polygon2D>("assets/textures/title.png", Near::Math::Vector2::Zero, Near::Math::Vector2(r->getWidth(), r->getHeight()));
  cameraPath.load("assets/levels/title-path.txt");
  camera->transform.position = cameraPath.getStartPosition();
  camera->transform.rotation = Near::createEularRotation(level->getSpawnRotation());
}

void SceneTitle::update(float deltaTime){
  PortalScene::update(deltaTime);
  time += deltaTime;
  cameraPath.advance(deltaTime);
  #ifdef _DEBUG
  if(Near::input()->isKeyPressedThisFrame('P')){
    camera->setDebugControlsEnabled(!camera->isDebugControlsEnabled());
    if(!camera->isDebugControlsEnabled()){
      camera->transform.position = cameraPath.getPosition();
    }
  }
  #endif
  if(!camera->isDebugControlsEnabled()){
    auto cameraMovement = cameraPath.getMovement();
    camera->move(cameraMovement, 1);
  }
  title->setColor(Near::Math::Color(1, 1, 1, std::clamp((time - 1000) / 1000, 0.0f, 1.0f)));
  if(Near::input()->isKeyPressedThisFrame(VK_ESCAPE)){
    Near::markClose();
    return;
  }
  if(Near::input()->isKeyPressedThisFrame(VK_SPACE)){
    NearGame::Game::Instance->levels.setNextLevel(0);
    NearGame::Game::Instance->fadeToNextScene<SceneGame>(NearGame::BACKGROUND_COLOR, 1000);
    return;
  }
  if(Near::input()->isKeyPressedThisFrame('G')){
    NearGame::Game::Instance->fadeToNextScene<SceneGUITest>(NearGame::BACKGROUND_COLOR, 1000);
    return;
  }
}

void SceneTitle::draw(){
  PortalScene::draw();
}

void SceneTitle::uninit(){
  level.reset();
  PortalScene::uninit();
}
