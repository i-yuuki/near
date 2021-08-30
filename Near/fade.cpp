#include "fade.h"

#include <algorithm>

Fade::Fade(){
}

void Fade::init(){
  state = State::NONE;
  vertices[0] = {Near::Math::Vector3(-1, -1, 0)};
  vertices[1] = {Near::Math::Vector3( 1, -1, 0)};
  vertices[2] = {Near::Math::Vector3(-1,  1, 0)};
  vertices[3] = {Near::Math::Vector3( 1,  1, 0)};
  vertexBuffer.init(true, 4, vertices);
  vertexShader.reset(new Near::VertexShader());
  vertexShader->load("assets/nearlib/shaders/vs.hlsl");
  pixelShader.reset(new Near::PixelShader());
  pixelShader->load("assets/shaders/ps-notex.hlsl");
}

void Fade::update(float deltaTime){
  if(state == State::NONE) return;
  time += deltaTime;
}

void Fade::draw(){
  if(state == State::NONE) return;

  float progress = getProgress();
  float alpha = std::clamp(state == State::FADE_IN ? 1 - progress : progress, 0.0f, 1.0f);
  auto color = this->color;
  color.w *= alpha;
  for(int i = 0;i < 4;i ++){
    vertices[i].color = color;
  }
  vertexBuffer.set(vertices);

  auto* r = Near::renderer();
  r->setVertexShader(vertexShader.get());
  r->setPixelShader(pixelShader.get());
  r->setWorldTransform(Near::Math::Matrix::Identity);
  r->setViewTransform(Near::Math::Matrix::Identity);
  r->setProjectionTransform(Near::Math::Matrix::Identity);
  vertexBuffer.draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

  if(state == State::FADE_IN && alpha == 0){
    state = State::NONE;
  }
}

void Fade::uninit(){
  vertexBuffer.uninit();
  vertexShader.reset();
  pixelShader.reset();
}

Fade::State Fade::getState() const{
  return state;
}

float Fade::getProgress() const{
  return state == State::NONE ? 0 : duration <= 0 ? 1 : time / duration;
}

void Fade::fadeIn(float duration){
  state = State::FADE_IN;
  time = 0;
  this->duration = duration;
}

void Fade::fadeOut(const Near::Math::Color& color, float duration){
  state = State::FADE_OUT;
  time = 0;
  this->duration = duration;
  this->color = color;
}
