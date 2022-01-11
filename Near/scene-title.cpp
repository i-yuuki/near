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
  camera->transform.position = level->getSpawnPosition();
  camera->transform.rotation = Near::createEularRotation(level->getSpawnRotation());
  camera->setFar(8000);
  title = getLayer(Near::Scene::LAYER_OVERLAY)->createGameObject<Polygon2D>("assets/textures/title.png", Near::Math::Vector2::Zero, Near::Math::Vector2(r->getWidth(), r->getHeight()));
  cameraPath.reset();
  cameraPath.addPointRelative(Near::Math::Vector3(0, -100, 0), Near::Math::Vector3(0, 0, 300), Near::Math::Vector3(0, 100, 0), 5000);
  cameraPath.addPointRelative(Near::Math::Vector3(0, -100, 0), Near::Math::Vector3(0, 0, -300), Near::Math::Vector3(0, 100, 0), 5000);
}

void SceneTitle::update(float deltaTime){
  PortalScene::update(deltaTime);
  time += deltaTime;
  cameraPath.advance(deltaTime);
  auto cameraMovement = cameraPath.getMovement();
  camera->move(cameraMovement, 1);
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
