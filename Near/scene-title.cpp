#include "pch.h"
#include "scene-title.h"

#include <algorithm>

#include <NearLib/gui-object.h>
#include <NearLib/gui/align.h>
#include <NearLib/gui/container.h>
#include <NearLib/gui/flex-container.h>
#include <NearLib/gui/text.h>
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
  camera->transform.position = level->getSpawnPosition();
  camera->transform.rotation = Near::createEularRotation(level->getSpawnRotation());

  auto gui = std::make_shared<Near::GUI::Align>(Near::Math::Vector2(0.5f, 0.65f));
  auto list = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::VERTICAL);
  list->setGap(16);

  auto addButton = [&list](const std::string& text, Near::Event::Signal<Near::GUI::MouseEvent>::Callback onClick){
    auto btn = std::make_shared<Near::GUI::Container>();
    btn->setWidth(Near::GUI::Length(300, Near::GUI::Unit::PX));
    btn->setBackground(Near::Math::Color(0.286f, 0.239f, 0.184f, 1.0f));
    
    auto label = std::make_shared<Near::GUI::Text>(text, Near::Assets::fonts()->get("Inter"));
    label->setFontSize(20);

    btn->add(Near::GUI::Align::Create(label, Near::Math::Vector2(0.5f)));
    btn->onMouseDown.addListener(onClick);
    list->add(btn);
  };

  addButton(u8"PLAY", [](const Near::GUI::MouseEvent& e){
    NearGame::Game::Instance->fadeToNextScene<SceneGame>(NearGame::BACKGROUND_COLOR, 1000);
  });
  addButton(u8"SETTINGS", [](const Near::GUI::MouseEvent& e){
    NearGame::Game::Instance->fadeToNextScene<SceneGUITest>(NearGame::BACKGROUND_COLOR, 1000);
  });
  addButton(u8"QUIT", [](const Near::GUI::MouseEvent& e){
    Near::markClose();
  });

  gui->setChild(list);
  gui->layout(Near::GUI::BoxConstraints(0, 0, r->getWidth(), r->getHeight()));
  getLayer(Near::Scene::LAYER_OVERLAY)->createGameObject<Near::GUIObject>(gui);
}

void SceneTitle::update(float deltaTime){
  PortalScene::update(deltaTime);
  time += deltaTime;
  camera->transform.position.y = 64 + std::sin(time / 10000) * 32;
  if(Near::input()->isKeyPressedThisFrame(VK_ESCAPE)){
    Near::markClose();
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
