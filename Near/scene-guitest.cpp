#include "scene-guitest.h"

#include <NearLib/camera.h>
#include <NearLib/utils.h>

#include "game.h"
#include "main.h"
#include "scene-title.h"

SceneGUITest::SceneGUITest() : Near::Scene(){
}

void SceneGUITest::init(){
  using Length = Near::GUI::Length;
  using Unit = Near::GUI::Unit;
  Near::Scene::init();
  getLayer(Near::Scene::LAYER_MANAGERS)->createGameObject<Near::Camera>();

  auto font = NearGame::Game::Instance->font;

  gui = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::VERTICAL);
  gui->setWidth(Length(100, Unit::PERCENT));
  gui->setHeight(Length(100, Unit::PERCENT));
  gui->setGap(16);
  gui->setBackground(NearGame::BACKGROUND_COLOR);

  auto text = std::make_shared<Near::GUI::Text>(u8"SETTINGS", font);
  text->setWidth(Length(100, Unit::PERCENT));
  text->setHeight(Length(48, Unit::PX));
  text->setFontSize(32);
  text->setTextAlign(Near::Math::Vector2(0.5f, 0.5f));
  text->setForeground(Near::Math::Color(0.286f, 0.239f, 0.184f, 1.0f));
  gui->add(text);

  auto tabs = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::HORIZONTAL);
  tabs->setWidth(Length(100, Unit::PERCENT));
  tabs->setHeight(Length(32, Unit::PX));
  tabs->setGap(32);
  auto addTab = [&tabs, &font](const std::string& name){
    auto text = std::make_shared<Near::GUI::Text>(name, font);
    text->setWidth(Length(100, Unit::PX));
    text->setHeight(Length(32, Unit::PX));
    text->setBackground(Near::Math::Color(0.286f, 0.239f, 0.184f, 1.0f));
    text->setTextAlign(Near::Math::Vector2(0.5f, 0.5f));
    tabs->add(text);
  };
  addTab(u8"GAMEPLAY");
  addTab(u8"AUDIO");
  addTab(u8"VIDEO");
  addTab(u8"CONTROLS");
  gui->add(tabs);

  auto line = std::make_shared<Near::GUI::Component>();
  line->setWidth(Length(100, Unit::PERCENT));
  line->setHeight(Length(2, Unit::PX));
  line->setBackground(Near::Math::Color(0.545f, 0.478f, 0.400f, 1.0f));
  gui->add(line);
  
  // flex test

  /*
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
  */

  gui->layout(Near::GUI::BoxConstraints(0, 0, 1280, 720));
}

void SceneGUITest::update(float deltaTime){
  Scene::update(deltaTime);
  time += deltaTime;
  if(Near::input()->isKeyPressedThisFrame(VK_ESCAPE)){
    NearGame::Game::Instance->fadeToNextScene<SceneTitle>(NearGame::BACKGROUND_COLOR, 1000);
    return;
  }
  // testText->setSize(Near::Math::Vector2(100 + std::sinf(time / 1500) * 100, std::abs(std::sinf(time / 1000))));
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
