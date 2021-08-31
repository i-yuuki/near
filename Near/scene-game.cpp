#include "scene-game.h"

#include <algorithm>

#include <NearLib/utils.h>

#include "game.h"
#include "main.h"
#include "player-camera.h"

SceneGame::SceneGame() : PortalScene(){
}

void SceneGame::init(){
  PortalScene::init();
  level.reset(new Level());
  level->load("testlevel.txt");
  getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<LevelObject>(level);
  level->createGameObjects(*this);
  auto player = getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<Player>();
  auto camera = getLayer(Near::Scene::LAYER_MANAGERS)->createGameObject<PlayerCamera>(player);
  player->transform.position = level->getSpawnPosition();
  player->transform.position.y += player->getSize().y / 2;
  auto spawnRotation = level->getSpawnRotation();
  player->transform.rotation = Near::createEularRotation(spawnRotation);
  camera->setFar(8000);
  Near::input()->lockMouse(true);
}

void SceneGame::update(float deltaTime){
  PortalScene::update(deltaTime);
#ifdef _DEBUG
  if(Near::input()->isKeyPressedThisFrame('R') && Near::input()->isKeyDown(VK_CONTROL)){
    NearGame::Game::Instance->fadeToNextScene<SceneGame>(NearGame::BACKGROUND_COLOR, 0);
  }
#endif
}

void SceneGame::draw(){
  PortalScene::draw();
}

void SceneGame::uninit(){
  PortalScene::uninit();
  Near::input()->lockMouse(false);
}
