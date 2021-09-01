#include <functional>
#include <iomanip>
#include <sstream>
#include <NearLib/near.h>
#include <NearLib/scene.h>
#include <NearLib/debug-grid.h>
#include <NearLib/model-object.h>
#include <NearLib/fps-counter.h>
#include <NearLib/dialog.h>

#include "game.h"
#include "portal-scene.h"
#include "portal.h"
#include "level-object.h"
#include "player.h"
#include "player-camera.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' \
  name='Microsoft.Windows.Common-Controls' \
  version='6.0.0.0' \
  processorArchitecture='*' \
  publicKeyToken='6595b64144ccf1df' \
  language='*'\"")

int APIENTRY WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmdShow){
#ifdef _DEBUG
  {
    AllocConsole();
    SetConsoleOutputCP(CP_UTF8);
    FILE *fp_c = NULL;
    freopen_s(&fp_c, "CONOUT$", "w", stdout);
    freopen_s(&fp_c, "CONIN$", "r", stdin);
  }
#endif
  
  try{
    Near::InitParams initParams;
    initParams.width = 1280;
    initParams.height = 720;
    initParams.windowTitle = u8"Near: Replica";
    Near::init(initParams);
  }catch(const std::exception& ex){
    Near::dialog::alert(u8"ゲームを開始できません", std::string(u8"初期化に失敗しました。ゲームファイルが壊れたか、対応していないPCかもしれません。\n\n").append(ex.what()), Near::dialog::DialogIcon::ERROR, u8"ゲームを終了");
    Near::uninit();
    return 1;
  }

  NearGame::Game::Instance = new NearGame::Game();
  NearGame::Game::Instance->init();

  while(!Near::shouldClose()){
    NearGame::Game::Instance->update();
    NearGame::Game::Instance->draw();
  }

  NearGame::Game::Instance->uninit();
  delete NearGame::Game::Instance;

  Near::uninit();

  return 0;
}
