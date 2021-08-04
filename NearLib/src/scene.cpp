#include "pch.h"
#include "scene.h"

namespace Near{

Scene::Scene(){
}

Scene::~Scene(){
  delete textures;
  delete vertexShaders;
  delete pixelShaders;
  uninit();
}

void Scene::init(){
  pixelShaders = new AssetManager<PixelShader>();
  vertexShaders = new AssetManager<VertexShader>();
  textures = new AssetManager<Texture>();
  insertLayer(LAYER_MANAGERS);
  insertLayer(LAYER_OBJECTS);
  insertLayer(LAYER_TRANSPARENT_OBJECTS);
  insertLayer(LAYER_OVERLAY);
  // for(auto& it : layers){
  //   it.second.init(this);
  // }
}

void Scene::beforeUpdate(float deltaTime){
  for(auto& it : layers){
    it.second.beforeUpdate(deltaTime);
  }
}

void Scene::update(float deltaTime){
  for(auto& it : layers){
    it.second.update(deltaTime);
  }
}

void Scene::afterUpdate(float deltaTime){
  for(auto& it : layers){
    it.second.afterUpdate(deltaTime);
  }
}

void Scene::draw(){
  for(auto& it : layers){
    it.second.draw();
  }
}

void Scene::uninit(){
  for(auto& it : layers){
    it.second.uninit();
  }
}

Layer* Scene::getLayer(int num){
  auto it = layers.find(num);
  return it == layers.end() ? nullptr : &it->second;
}

Layer* Scene::insertLayer(int num){
  // try_emplace:
  // 要素があったら<既存の要素, false>、なかったら挿入して<挿入した要素, true>のpairを返す
  auto res = layers.try_emplace(num);
  if(res.second){
    res.first->second.init(this);
  }
  return &res.first->second;
}

void Scene::deleteLayer(int num){
  auto it = layers.find(num);
  if(it == layers.end()) return;
  it->second.uninit();
  layers.erase(it);
}

}
