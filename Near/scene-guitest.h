#pragma once

#include <NearLib/scene.h>
#include <NearLib/gui/container.h>
#include <NearLib/gui/text.h>

class SceneGUITest : public Near::Scene{
public:
  SceneGUITest();
  virtual void init() override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
private:
  float time;
  std::shared_ptr<Near::GUI::Container> gui;
  std::shared_ptr<Near::GUI::Text> testText;
};
