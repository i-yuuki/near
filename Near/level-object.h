#pragma once

#include <NearLib/game-object.h>
#include <NearLib/buffer.h>
#include <NearLib/shader.h>

#include "level.h"

class LevelObject : public Near::GameObject{
public:
  LevelObject(std::shared_ptr<Level> level);
  virtual void init(Near::Layer* layer) override;
  virtual void draw() override;
  virtual void uninit() override;
  Level* getLevel();
private:
  std::shared_ptr<Level> level;
  Near::StandardVertexBuffer vertexBuffer;
  Near::IndexBuffer indexBuffer;
  Near::IndexBuffer indexBufferWireframe;
  std::shared_ptr<Near::VertexShader> vertexShader;
  std::shared_ptr<Near::PixelShader> pixelShader;
  std::shared_ptr<Near::PixelShader> pixelShaderWireframe;
};
