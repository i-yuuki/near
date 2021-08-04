#include "pch.h"
#include "model-object.h"

#include "scene.h"
#include "utils.h"

namespace Near{

ModelObject::ModelObject(const char* modelPath) : modelPath(modelPath){
}

void ModelObject::init(Layer* layer){
  super::init(layer);
  model = new FBXModel();
  model->load(modelPath);
  vertexShader = layer->getScene()->vertexShaders->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = layer->getScene()->pixelShaders->getOrLoad("assets/nearlib/shaders/ps.hlsl");
}

void ModelObject::draw(){
  auto* r = renderer();
  r->setVertexShader(vertexShader.get());
  r->setPixelShader(pixelShader.get());
  Math::Matrix t = transform.createTransform();
  model->draw(&t);
}

void ModelObject::uninit(){
  safeDelete(model);
  vertexShader.reset();
  pixelShader.reset();
}

}
