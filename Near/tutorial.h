#pragma once

#include "polygon-2d.h"

class Tutorial : public Polygon2D{
public:
  Tutorial(Near::Math::Vector2 position, Near::Math::Vector2 size, const std::string* textures, int textureCount);
  virtual void init(Near::Layer* layer) override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
private:
  float time;
  std::vector<std::string> texturePaths;
  std::vector<std::shared_ptr<Near::Texture>> textures;
};
