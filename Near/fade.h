#pragma once

#include <NearLib/buffer.h>

class Fade{
public:
  enum class State{
    NONE,
    FADE_IN,
    FADE_OUT,
  };
  Fade();
  void init();
  void update(float deltaTime);
  void draw();
  void uninit();
  State getState() const;
  float getProgress() const;
  // シーンをフェードインさせます (オーバーレイをフェードアウト)
  void fadeIn(float duration);
  // シーンをフェードアウトさせます (オーバーレイをフェードイン)
  void fadeOut(const Near::Math::Color& color, float duration);
private:
  State state;
  float time;
  float duration;
  Near::Math::Color color{0, 0, 0, 1};
  Near::Vertex3D vertices[4];
  Near::StandardVertexBuffer vertexBuffer;
  std::unique_ptr<Near::VertexShader> vertexShader;
  std::unique_ptr<Near::PixelShader> pixelShader;
};
