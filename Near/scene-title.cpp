#include "scene-title.h"

#include <algorithm>

#include <NearLib/debug-camera.h>

#include "game.h"
#include "main.h"
#include "polygon-2d.h"
#include "scene-game.h"

SceneTitle::SceneTitle() : PortalScene(){
}

void SceneTitle::init(){
  PortalScene::init();
  auto* r = Near::renderer();
  getLayer(Near::Scene::LAYER_MANAGERS)->createGameObject<Near::DebugCamera>();
  title = getLayer(Near::Scene::LAYER_OVERLAY)->createGameObject<Polygon2D>("assets/textures/title.png", Near::Math::Vector2::Zero, Near::Math::Vector2(r->getWidth(), r->getHeight()));
}

void SceneTitle::update(float deltaTime){
  PortalScene::update(deltaTime);
  time += deltaTime;
  title->setColor(Near::Math::Color(1, 1, 1, std::clamp((time - 1000) / 1000, 0.0f, 1.0f)));
  if(Near::input()->isKeyPressedThisFrame(VK_SPACE)){
    NearGame::Game::Instance->setNextScene<SceneGame>();
  }
}

void SceneTitle::draw(){
  PortalScene::draw();
}

void SceneTitle::uninit(){
  PortalScene::uninit();
}
