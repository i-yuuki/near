#include "scene-game.h"

#include <algorithm>

#include "game.h"
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
  player->transform.position.y = 500;
  camera->setFar(8000);
}

void SceneGame::update(float deltaTime){
  PortalScene::update(deltaTime);
}

void SceneGame::draw(){
  PortalScene::draw();
}

void SceneGame::uninit(){
  PortalScene::uninit();
}
