#pragma once

#include <NearLib/fps-counter.h>
#include <NearLib/scene.h>

namespace NearGame{

class Game{
public:
  static Game* Instance;
  Game();
  void init();
  void update();
  void draw();
  void uninit();
  template<class T, typename... Args>
  void setNextScene(Args&&... args){
    if(nextScene){
      delete nextScene;
    }
    nextScene = new T(std::forward<Args>(args)...);
  }
  Near::FPSCounter timer;
private:
  Near::Scene* scene = nullptr;
  Near::Scene* nextScene = nullptr;
};

}
