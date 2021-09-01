#include "scene-game.h"

#include <algorithm>

#include <NearLib/utils.h>

#include "game.h"
#include "main.h"
#include "player-camera.h"
#include "tutorial.h"

SceneGame::SceneGame() : PortalScene(){
}

void SceneGame::init(){
  PortalScene::init();
  NearGame::Game::Instance->levels.nextLevel();
  auto level = NearGame::Game::Instance->levels.getLevel();
  getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<LevelObject>(level);
  level->createGameObjects(*this);
  auto player = getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<Player>();
  auto camera = getLayer(Near::Scene::LAYER_MANAGERS)->createGameObject<PlayerCamera>(player);
  player->transform.position = level->getSpawnPosition();
  player->transform.position.y += player->getSize().y / 2;
  auto spawnRotation = level->getSpawnRotation();
  player->transform.rotation = Near::createEularRotation(spawnRotation);
  camera->setFar(8000);
  std::string tutorialTextures[] = {
    "assets/textures/tutorial-look.png",
    "assets/textures/tutorial-move.png",
    "assets/textures/tutorial-interact.png",
  };
  getLayer(Near::Scene::LAYER_OVERLAY)->createGameObject<Tutorial>(Near::Math::Vector2(440, 600), Near::Math::Vector2(400, 104), tutorialTextures, 3);
  Near::input()->lockMouse(true);
}

void SceneGame::update(float deltaTime){
  PortalScene::update(deltaTime);
#ifdef _DEBUG
  if(Near::input()->isKeyPressedThisFrame('R') && Near::input()->isKeyDown(VK_CONTROL)){
    auto* game = NearGame::Game::Instance;
    game->levels.setNextLevel(game->levels.getLevelIdx());
    game->fadeToNextScene<SceneGame>(NearGame::BACKGROUND_COLOR, 0);
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
