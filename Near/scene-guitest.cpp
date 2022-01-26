#include "scene-guitest.h"

#include <NearLib/camera.h>
#include <NearLib/utils.h>
#include <NearLib/gui/container.h>
#include <NearLib/gui/flexible.h>

#include "game.h"
#include "main.h"
#include "scene-title.h"

SettingsPage::SettingsPage(const std::string& title){
  root = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::VERTICAL);
  root->setGap(16);
  auto font = Near::Assets::fonts()->get("Inter");
  auto text = std::make_shared<Near::GUI::Text>(title, font);
  text->setFontSize(32);
  text->setTextAlign(Near::Math::Vector2(0.0f, 0.5f));
  text->setForeground(Near::Math::Color(0.286f, 0.239f, 0.184f, 1.0f));
  root->add(text);
}

void SettingsPage::addItem(const std::string& name, const std::string& value){
  auto font = Near::Assets::fonts()->get("Inter");
  auto item = std::make_shared<Near::GUI::Container>();
  item->setWidth(Near::GUI::Length(400, Near::GUI::Unit::PX));
  item->setBackground(Near::Math::Color(0.286f, 0.239f, 0.184f, 0.25f));
  auto text = std::make_shared<Near::GUI::Text>(name, font);
  text->setTextAlign(Near::Math::Vector2(0.0f, 0.5f));
  text->setForeground(Near::Math::Color(0.286f, 0.239f, 0.184f, 1.0f));
  item->add(text);
  text = std::make_shared<Near::GUI::Text>(value, font);
  text->setTextAlign(Near::Math::Vector2(1.0f, 0.5f));
  text->setForeground(Near::Math::Color(0.286f, 0.239f, 0.184f, 1.0f));
  item->add(text);
  root->add(item);
}

SceneGUITest::SceneGUITest() : Near::Scene(){
}

void SceneGUITest::init(){
  using Length = Near::GUI::Length;
  using Unit = Near::GUI::Unit;
  Near::Scene::init();
  getLayer(Near::Scene::LAYER_MANAGERS)->createGameObject<Near::Camera>();

  auto font = Near::Assets::fonts()->get("Inter");

  gui = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::VERTICAL);
  gui->setGap(16);

  auto text = std::make_shared<Near::GUI::Text>(u8"SETTINGS", font);
  text->setFontSize(32);
  text->setTextAlign(Near::Math::Vector2(0.5f, 0.5f));
  text->setForeground(Near::Math::Color(0.286f, 0.239f, 0.184f, 1.0f));
  gui->add(text);

  auto tabs = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::HORIZONTAL);
  tabs->setGap(32);
  auto addTab = [this, &tabs, &font](const std::string& name){
    auto text = std::make_shared<Near::GUI::Text>(name, font);
    text->setTextAlign(Near::Math::Vector2(0.5f, 0.5f));
    auto tab = std::make_shared<Near::GUI::Flexible>(1, text);
    tab->setBackground(Near::Math::Color(0.286f, 0.239f, 0.184f, 1.0f));
    tabs->add(tab);
    return tab;
  };
  pageGameplay.tab = addTab(u8"GAMEPLAY");
  pageAudio.tab = addTab(u8"AUDIO");
  pageVideo.tab = addTab(u8"VIDEO");
  pageControls.tab = addTab(u8"CONTROLS");
  gui->add(tabs);

  auto line = std::make_shared<Near::GUI::Container>();
  line->setHeight(Length(2, Unit::PX));
  line->setBackground(Near::Math::Color(0.545f, 0.478f, 0.400f, 1.0f));
  gui->add(line);

  // 設定リスト実験

  pageContainer = std::make_shared<Near::GUI::Container>();

  auto addScreen = [this, &font](const std::string& name){
    auto screen = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::VERTICAL);
    screen->setGap(16);
    auto text = std::make_shared<Near::GUI::Text>(name, font);
    text->setFontSize(32);
    text->setTextAlign(Near::Math::Vector2(0.0f, 0.5f));
    text->setForeground(Near::Math::Color(0.286f, 0.239f, 0.184f, 1.0f));
    screen->add(text);
    return screen;
  };

  pageGameplay.page = std::make_shared<SettingsPage>(u8"ゲームプレイ設定");
  pageAudio.page = std::make_shared<SettingsPage>(u8"オーディオ設定");
  pageVideo.page = std::make_shared<SettingsPage>(u8"ビデオ設定");
  pageControls.page = std::make_shared<SettingsPage>(u8"操作設定");

  pageGameplay.page->addItem(u8"■ 視野角",           u8"90");
  pageGameplay.page->addItem(u8"■ 解像度",           u8"1920x1080");
  pageGameplay.page->addItem(u8"■ 画面",             u8"ボーダーレス");
  pageGameplay.page->addItem(u8"■ 垂直同期",         u8"ON");
  pageGameplay.page->addItem(u8"■ アンチエイリアス", u8"ON");
  pageGameplay.page->addItem(u8"■ GUIスケール",      u8"1.00");

  gui->add(pageContainer);

  pageGameplay.tab->onMouseDown.addListener([this](Near::GUI::MouseEvent e){ setPage(pageGameplay); });
  pageAudio.tab->onMouseDown.addListener([this](Near::GUI::MouseEvent e){ setPage(pageAudio); });
  pageVideo.tab->onMouseDown.addListener([this](Near::GUI::MouseEvent e){ setPage(pageVideo); });
  pageControls.tab->onMouseDown.addListener([this](Near::GUI::MouseEvent e){ setPage(pageControls); });
  setPage(pageGameplay);

  Near::input()->onMouseDown.addListener([this](Near::InputManager::MouseEvent e){
    Near::Math::Vector2 mousePos(e.x, e.y);
    if(auto c = gui->getDeepComponentAt(mousePos)){
      c->propagateMouseDownEvent(Near::GUI::MouseEvent(c, mousePos, e.button));
    }
  });

  // flex test

  /*
  auto sizetest = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::HORIZONTAL);
  sizetest->setWidth(Length(500, Unit::PX));
  sizetest->setHeight(Length(300, Unit::PX));
  sizetest->setBackground(Near::Math::Color(1, 1, 1, 0.7f));
  testText = std::make_shared<Near::GUI::Text>(u8"幅変えてる", font);
  testText->setWidth(Length(100, Unit::PX));
  testText->setHeight(Length(50, Unit::PERCENT));
  testText->setBackground(Near::Math::Color(0, 0, 0, 0.5f));
  sizetest->add(testText);

  text = std::make_shared<Near::GUI::Text>(u8"幅固定", font);
  text->setWidth(Length(100, Unit::PX));
  text->setHeight(Length(50, Unit::PX));
  text->setBackground(Near::Math::Color(1, 0, 0, 0.5f));
  sizetest->add(text);

  text = std::make_shared<Near::GUI::Text>(u8"幅固定", font);
  text->setWidth(Length(100, Unit::PX));
  text->setHeight(Length(50, Unit::PX));
  text->setBackground(Near::Math::Color(1, 1, 0, 0.5f));
  sizetest->add(text);

  auto col = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::VERTICAL);
  col->setHeight(Length(100, Unit::PERCENT));
  col->setBackground(Near::Math::Color(0, 0, 1, 0.5f));
  text = std::make_shared<Near::GUI::Text>(u8"残りの幅", font);
  text->setHeight(Length(30, Unit::PX));
  col->add(text);
  text = std::make_shared<Near::GUI::Text>(u8"残りの高さ2", font);
  text->setBackground(Near::Math::Color(0, 1, 0, 0.5f));
  col->add(std::make_shared<Near::GUI::Flexible>(2, text));
  text = std::make_shared<Near::GUI::Text>(u8"残りの高さ1", font);
  text->setBackground(Near::Math::Color(0, 0.8f, 0, 0.5f));
  col->add(std::make_shared<Near::GUI::Flexible>(1, text));
  sizetest->add(std::make_shared<Near::GUI::Flexible>(1, col));

  col = std::make_shared<Near::GUI::FlexContainer>(Near::GUI::FlexContainer::Direction::VERTICAL);
  col->setHeight(Length(100, Unit::PERCENT));
  col->setBackground(Near::Math::Color(1, 0, 0, 0.5f));

  text = std::make_shared<Near::GUI::Text>(u8"残りの幅", font);
  text->setHeight(Length(30, Unit::PX));
  col->add(text);
  text = std::make_shared<Near::GUI::Text>(u8"吾", font);
  text->setHeight(Length(30, Unit::PX));
  text->setTextAlign(Near::Math::Vector2(0.5f, 0.0f));
  col->add(text);
  text = std::make_shared<Near::GUI::Text>(u8"輩", font);
  text->setHeight(Length(30, Unit::PX));
  text->setTextAlign(Near::Math::Vector2(0.5f, 0.0f));
  col->add(text);
  text = std::make_shared<Near::GUI::Text>(u8"は", font);
  text->setHeight(Length(30, Unit::PX));
  text->setTextAlign(Near::Math::Vector2(0.5f, 0.0f));
  col->add(text);
  text = std::make_shared<Near::GUI::Text>(u8"猫", font);
  text->setHeight(Length(30, Unit::PX));
  text->setTextAlign(Near::Math::Vector2(0.5f, 0.0f));
  col->add(text);
  sizetest->add(std::make_shared<Near::GUI::Flexible>(1, col));
  // gui->add(sizetest);
  */
}

void SceneGUITest::update(float deltaTime){
  Scene::update(deltaTime);
  time += deltaTime;
  if(Near::input()->isKeyPressedThisFrame(VK_ESCAPE)){
    NearGame::Game::Instance->fadeToNextScene<SceneTitle>(NearGame::BACKGROUND_COLOR, 1000);
    return;
  }
  // testText->setWidth(Near::GUI::Length(100 + std::sinf(time / 1500) * 100, Near::GUI::Unit::PX));
  // testText->setHeight(Near::GUI::Length(std::abs(std::sinf(time / 1000)) * 100, Near::GUI::Unit::PERCENT));
  // TODO setSizeでレイアウトもさせる
  gui->layout(Near::GUI::BoxConstraints(0, 0, 1280, 720));
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

void SceneGUITest::setPage(PageInfo& page){
  if(currentPage.page){
    pageContainer->remove(currentPage.page->root);
  }
  currentPage = page;
  pageContainer->add(currentPage.page->root);
}
