#pragma once

#include "game-object.h"
#include "buffer.h"
#include "vertex.h"

namespace Near{

class DebugGrid : public GameObject{
public:
  DebugGrid(unsigned int lineCount);
  virtual void init(Layer* layer) override;
  virtual void draw() override;
  virtual void uninit() override;
private:
  unsigned int lineCount;
  unsigned int vertexCount;
  Vertex3D* vertices;
  int lastOriginX;
  int lastOriginY;
  int lastOriginZ;
  VertexBuffer vertexBuffer;
  std::shared_ptr<VertexShader> vertexShader;
  std::shared_ptr<PixelShader> pixelShader;
  void updateVertexBuffer(bool force = false);
};

}
