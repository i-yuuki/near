#pragma once

#include <NearLib/scene.h>
#include <NearLib/gui/flex-container.h>
#include <NearLib/gui/text.h>

class SettingsPage{
public:
  SettingsPage(const std::string& title);
  void addItem(const std::string& name, const std::string& value);
  std::shared_ptr<Near::GUI::FlexContainer> root;
};

class SceneGUITest : public Near::Scene{
public:
  SceneGUITest();
  virtual void init() override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
private:
  struct PageInfo{
    std::shared_ptr<Near::GUI::Component> tab;
    std::shared_ptr<SettingsPage> page;
  };
  void setPage(PageInfo& page);
  float time;
  std::shared_ptr<Near::GUI::Component> gui;
  std::shared_ptr<Near::GUI::Container> pageContainer;
  PageInfo currentPage{};
  PageInfo pageGameplay;
  PageInfo pageAudio;
  PageInfo pageVideo;
  PageInfo pageControls;
};
