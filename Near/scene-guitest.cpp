#include "scene-guitest.h"

#include <NearLib/camera.h>
#include <NearLib/utils.h>
#include <NearLib/gui/flex-container.h>

#include "game.h"
#include "main.h"
#include "scene-title.h"

SceneGUITest::SceneGUITest() : Near::Scene(){
}

void SceneGUITest::init(){
  Near::Scene::init();
  getLayer(Near::Scene::LAYER_MANAGERS)->createGameObject<Near::Camera>();

  auto font = NearGame::Game::Instance->font;

  gui = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::VERTICAL);
  gui->setWidthUnit(Near::GUI::SizeUnit::PARENT);
  gui->setHeightUnit(Near::GUI::SizeUnit::PARENT);
  gui->setBackground(NearGame::BACKGROUND_COLOR);

  auto text = std::make_shared<Near::GUI::Text>(u8"SETTINGS", font);
  text->setSize(Near::Math::Vector2(1, 48));
  text->setWidthUnit(Near::GUI::SizeUnit::PARENT);
  text->setFontSize(32);
  text->setTextAlign(Near::Math::Vector2(0.5f, 0.5f));
  gui->add(text);

  auto tabs = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::HORIZONTAL);
  tabs->setSize(Near::Math::Vector2(1, 64));
  tabs->setWidthUnit(Near::GUI::SizeUnit::PARENT);
  auto addTab = [&tabs, &font](const std::string& name){
    auto text = std::make_shared<Near::GUI::Text>(name, font);
    text->setSize(Near::Math::Vector2(1, 32));
    text->setWidthUnit(Near::GUI::SizeUnit::FILL_CONTAINER);
    text->setBackground(Near::Math::Color(0.286f, 0.239f, 0.184f, 1.0f));
    text->setTextAlign(Near::Math::Vector2(0.5f, 0.5f));
    tabs->add(text);
  };
  addTab(u8"GAMEPLAY");
  addTab(u8"AUDIO");
  addTab(u8"VIDEO");
  addTab(u8"CONTROLS");
  gui->add(tabs);
  
  // flex test

  auto sizetest = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::HORIZONTAL);
  sizetest->setSize(Near::Math::Vector2(500, 300));
  sizetest->setBackground(Near::Math::Color(1, 1, 1, 0.7f));
  testText = std::make_shared<Near::GUI::Text>(u8"幅変えてる", font);
  testText->setSize(Near::Math::Vector2(100, 0.5f));
  testText->setHeightUnit(Near::GUI::SizeUnit::PARENT);
  testText->setBackground(Near::Math::Color(0, 0, 0, 0.5f));
  sizetest->add(testText);

  text = std::make_shared<Near::GUI::Text>(u8"幅固定", font);
  text->setSize(Near::Math::Vector2(100, 50));
  text->setBackground(Near::Math::Color(1, 0, 0, 0.5f));
  sizetest->add(text);

  text = std::make_shared<Near::GUI::Text>(u8"幅固定", font);
  text->setSize(Near::Math::Vector2(100, 50));
  text->setBackground(Near::Math::Color(1, 1, 0, 0.5f));
  sizetest->add(text);

  auto col = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::VERTICAL);
  col->setWidthUnit(Near::GUI::SizeUnit::FILL_CONTAINER);
  col->setHeightUnit(Near::GUI::SizeUnit::PARENT);
  col->setBackground(Near::Math::Color(0, 0, 1, 0.5f));
  text = std::make_shared<Near::GUI::Text>(u8"残りの幅", font);
  text->setSize(Near::Math::Vector2(1, 30));
  text->setWidthUnit(Near::GUI::SizeUnit::PARENT);
  col->add(text);
  text = std::make_shared<Near::GUI::Text>(u8"残りの高さ2", font);
  text->setSize(Near::Math::Vector2(1, 2));
  text->setWidthUnit(Near::GUI::SizeUnit::PARENT);
  text->setHeightUnit(Near::GUI::SizeUnit::FILL_CONTAINER);
  text->setBackground(Near::Math::Color(0, 1, 0, 0.5f));
  col->add(text);
  text = std::make_shared<Near::GUI::Text>(u8"残りの高さ1", font);
  text->setSize(Near::Math::Vector2(1, 1));
  text->setWidthUnit(Near::GUI::SizeUnit::PARENT);
  text->setHeightUnit(Near::GUI::SizeUnit::FILL_CONTAINER);
  text->setBackground(Near::Math::Color(0, 0.8f, 0, 0.5f));
  col->add(text);
  sizetest->add(col);

  col = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::VERTICAL);
  col->setWidthUnit(Near::GUI::SizeUnit::FILL_CONTAINER);
  col->setHeightUnit(Near::GUI::SizeUnit::PARENT);
  col->setBackground(Near::Math::Color(1, 0, 0, 0.5f));

  text = std::make_shared<Near::GUI::Text>(u8"残りの幅", font);
  text->setSize(Near::Math::Vector2(1, 30));
  text->setWidthUnit(Near::GUI::SizeUnit::PARENT);
  col->add(text);
  text = std::make_shared<Near::GUI::Text>(u8"吾", font);
  text->setSize(Near::Math::Vector2(1, 30));
  text->setWidthUnit(Near::GUI::SizeUnit::PARENT);
  text->setTextAlign(Near::Math::Vector2(0.5f, 0.0f));
  col->add(text);
  text = std::make_shared<Near::GUI::Text>(u8"輩", font);
  text->setSize(Near::Math::Vector2(1, 30));
  text->setWidthUnit(Near::GUI::SizeUnit::PARENT);
  text->setTextAlign(Near::Math::Vector2(0.5f, 0.0f));
  col->add(text);
  text = std::make_shared<Near::GUI::Text>(u8"は", font);
  text->setSize(Near::Math::Vector2(1, 30));
  text->setWidthUnit(Near::GUI::SizeUnit::PARENT);
  text->setTextAlign(Near::Math::Vector2(0.5f, 0.0f));
  col->add(text);
  text = std::make_shared<Near::GUI::Text>(u8"猫", font);
  text->setSize(Near::Math::Vector2(1, 30));
  text->setWidthUnit(Near::GUI::SizeUnit::PARENT);
  text->setTextAlign(Near::Math::Vector2(0.5f, 0.0f));
  col->add(text);
  sizetest->add(col);

  gui->add(sizetest);
  gui->layout();
}

void SceneGUITest::update(float deltaTime){
  Scene::update(deltaTime);
  time += deltaTime;
  if(Near::input()->isKeyPressedThisFrame(VK_ESCAPE)){
    NearGame::Game::Instance->fadeToNextScene<SceneTitle>(NearGame::BACKGROUND_COLOR, 1000);
    return;
  }
  testText->setSize(Near::Math::Vector2(100 + std::sinf(time / 1500) * 100, std::abs(std::sinf(time / 1000))));
  // TODO setSizeでレイアウトもさせる
  // gui->layout();
}

void SceneGUITest::draw(){
  Near::Scene::draw();
  Near::renderer2D()->begin();
  gui->draw();
  Near::renderer2D()->end();
}

void SceneGUITest::uninit(){
  gui.reset();
  Near::Scene::uninit();
}
