#pragma once

#include "game-object.h"
#include "fbx-model.h"

namespace Near{

class ModelObject : public GameObject{
public:
  ModelObject(const char* modelPath);
  virtual void init(Layer* layer) override;
  virtual void draw() override;
  virtual void uninit() override;
private:
  const char* modelPath;
  FBXModel* model;
  std::shared_ptr<VertexShader> vertexShader;
  std::shared_ptr<PixelShader> pixelShader;
};

}
