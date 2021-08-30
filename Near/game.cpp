#include "game.h"

#include "main.h"
#include "scene-title.h"

namespace NearGame{

Game* Game::Instance = nullptr;

Game::Game(){
}

void Game::init(){
  setNextScene<SceneTitle>();
}

void Game::update(){
  timer.frame();
  Near::pollEvents();
  if(nextScene){
    if(scene){
      scene->uninit();
      delete scene;
    }
    scene = nextScene;
    nextScene = nullptr;
    scene->init();
  }
  if(scene){
    scene->beforeUpdate(timer.getLastFrameTime());
    scene->update(timer.getLastFrameTime());
    scene->afterUpdate(timer.getLastFrameTime());
  }
}

void Game::draw(){
  auto* r = Near::renderer();
  r->clear(BACKGROUND_COLOR);
  r->setWorldTransform(Near::Math::Matrix::Identity);
  if(scene){
    scene->draw();
  }
  r->present();
}

void Game::uninit(){
  if(scene){
    scene->uninit();
  }
}

}
