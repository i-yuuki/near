#include "game.h"

#include "main.h"
#include "scene-title.h"

namespace NearGame{

Game* Game::Instance = nullptr;

Game::Game(){
}

void Game::init(){
  levels.init("assets/levels.txt");
  fade.init();
  fadeToNextScene<SceneTitle>(Near::Math::Color(0, 0, 0, 1), 0);
}

void Game::update(){
  timer.frame();
  Near::pollEvents();
  fade.update(timer.getLastFrameTime());
  if(nextScene){
    if(fade.getState() == Fade::State::FADE_OUT){
      if(fade.getProgress() < 1) return;
      fade.fadeIn(1000);
    }
    if(scene){
      scene->uninit();
      delete scene;
    }
    scene = nextScene;
    nextScene = nullptr;
    scene->init();
    Near::Assets::vertexShaders()->unloadUnused();
    Near::Assets::pixelShaders()->unloadUnused();
    Near::Assets::textures()->unloadUnused();
    // deltaTimeをクリア (ちょっとゴリ押し感ある)
    timer.frame();
    timer.frame();
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
  fade.draw();
  r->present();
}

void Game::uninit(){
  if(scene){
    scene->uninit();
  }
}

}
