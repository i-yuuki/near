#include "pch.h"
#include "scene-finish.h"

#include "game.h"
#include "main.h"
#include "polygon-2d.h"
#include "scene-title.h"

SceneFinish::SceneFinish() : Scene(){
}

void SceneFinish::init(){
  Scene::init();
  auto* r = Near::renderer();
  getLayer(Near::Scene::LAYER_OVERLAY)->createGameObject<Polygon2D>("assets/textures/finish.png", Near::Math::Vector2::Zero, Near::Math::Vector2(r->getWidth(), r->getHeight()));
}

void SceneFinish::update(float deltaTime){
  Scene::update(deltaTime);
  if(Near::input()->isKeyPressedThisFrame(VK_SPACE)){
    NearGame::Game::Instance->fadeToNextScene<SceneTitle>(NearGame::BACKGROUND_COLOR, 1000);
  }
}
