#include "tutorial.h"

#include <NearLib/scene.h>

Tutorial::Tutorial(Near::Math::Vector2 position, Near::Math::Vector2 size, const std::string* textures, int textureCount)
: Polygon2D(textures[0], position, size){
  for(int i = 0;i < textureCount;i ++){
    texturePaths.push_back(textures[i]);
  }
}

void Tutorial::init(Near::Layer* layer){
  Polygon2D::init(layer);
  time = 0;
  textures.resize(texturePaths.size());
  for(int i = 0;i < texturePaths.size();i ++){
    textures[i] = layer->getScene()->textures->getOrLoad(texturePaths[i]);
  }
}

void Tutorial::update(float deltaTime){
  time += deltaTime;
}

void Tutorial::draw(){
  static constexpr float FRAME_DURATION = 5000;
  static constexpr float FRAME_FADE = 300;
  int textureIdx = static_cast<int>(std::floor(time / FRAME_DURATION));
  if(textureIdx >= textures.size()) return;
  float frameTime = std::fmod(time, FRAME_DURATION);
  float alpha = 1;
  if(frameTime < FRAME_FADE){
    alpha = frameTime / FRAME_FADE;
  }else if(frameTime > FRAME_DURATION - FRAME_FADE){
    alpha = (FRAME_DURATION - frameTime) / FRAME_FADE;
  }
  setTexture(textures[textureIdx]);
  setColor(Near::Math::Color(1, 1, 1, alpha));
  Polygon2D::draw();
}

void Tutorial::uninit(){
  textures.clear();
  textures.shrink_to_fit();
}
