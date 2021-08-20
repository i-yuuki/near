#include <functional>
#include <iomanip>
#include <sstream>
#include <NearLib/near.h>
#include <NearLib/scene.h>
#include <NearLib/debug-grid.h>
#include <NearLib/model-object.h>
#include <NearLib/fps-counter.h>
#include <NearLib/dialog.h>

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
  {
    AllocConsole();
    SetConsoleOutputCP(CP_UTF8);
    FILE *fp_c = NULL;
    freopen_s(&fp_c, "CONOUT$", "w", stdout);
    freopen_s(&fp_c, "CONIN$", "r", stdin);
  }
  
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

  PortalScene scene;
  try{
    scene.init();
    // auto camera = scene.getLayer(Near::Scene::LAYER_MANAGERS)->createGameObject<PortalCamera>();
    // auto level = scene.getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<Near::ModelObject>("assets/models/untitled.fbx");
    auto grid = scene.getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<Near::DebugGrid>(20);
    auto level = scene.getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<LevelObject>("testlevel.txt");
    auto player = scene.getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<Player>();
    auto camera = scene.getLayer(Near::Scene::LAYER_MANAGERS)->createGameObject<PlayerCamera>(player);
    auto portalA = scene.getLayer(Near::Scene::LAYER_TRANSPARENT_OBJECTS)->createGameObject<Portal>(Near::Math::Vector2(64, 64), Near::Math::Color(1.000f, 0.341f, 0.133f, 1.0f));
    auto portalB = scene.getLayer(Near::Scene::LAYER_TRANSPARENT_OBJECTS)->createGameObject<Portal>(Near::Math::Vector2(64, 64), Near::Math::Color(1.000f, 0.341f, 0.133f, 1.0f));
    auto portalC = scene.getLayer(Near::Scene::LAYER_TRANSPARENT_OBJECTS)->createGameObject<Portal>(Near::Math::Vector2(64, 64), Near::Math::Color(0.298f, 0.686f, 0.314f, 1.0f));
    auto portalD = scene.getLayer(Near::Scene::LAYER_TRANSPARENT_OBJECTS)->createGameObject<Portal>(Near::Math::Vector2(64, 64), Near::Math::Color(0.298f, 0.686f, 0.314f, 1.0f));
    auto portalE = scene.getLayer(Near::Scene::LAYER_TRANSPARENT_OBJECTS)->createGameObject<Portal>(Near::Math::Vector2(64, 64), Near::Math::Color(0.247f, 0.318f, 0.710f, 1.0f));
    auto portalF = scene.getLayer(Near::Scene::LAYER_TRANSPARENT_OBJECTS)->createGameObject<Portal>(Near::Math::Vector2(64, 64), Near::Math::Color(0.247f, 0.318f, 0.710f, 1.0f));
    
    camera->setFar(8000);

    // level->transform.position = Near::Math::Vector3(-136.16f, -0.17f, -81.07f);
    // level->transform.scale = Near::Math::Vector3(1 / 6400.0f);

    player->transform.position = Near::Math::Vector3(0, 500, 0);

    portalA->transform.position = Near::Math::Vector3(0, 64, 256);
    portalA->transform.rotation = Near::Math::Quaternion::CreateFromYawPitchRoll(0, 0, 0);
    portalA->otherPortal = portalB;

    portalB->transform.position = Near::Math::Vector3(0, 64, -256);
    portalB->transform.rotation = Near::Math::Quaternion::CreateFromYawPitchRoll(DirectX::XM_PI, 0, 0);
    portalB->otherPortal = portalA;

    portalC->transform.position = Near::Math::Vector3(160, 64, 256);
    portalC->transform.rotation = Near::Math::Quaternion::CreateFromYawPitchRoll(0, 0, 0);
    portalC->otherPortal = portalD;

    portalD->transform.position = Near::Math::Vector3(300, 64, 0);
    portalD->transform.rotation = Near::Math::Quaternion::CreateFromYawPitchRoll(DirectX::XM_PIDIV2, 0, 0);
    portalD->otherPortal = portalC;

    portalE->transform.position = Near::Math::Vector3(0, -128, 384);
    portalE->transform.rotation = Near::Math::Quaternion::CreateFromYawPitchRoll(0, DirectX::XM_PIDIV2, 0);
    portalE->otherPortal = portalF;

    portalF->transform.position = Near::Math::Vector3(-128, 64, -256);
    portalF->transform.rotation = Near::Math::Quaternion::CreateFromYawPitchRoll(DirectX::XM_PI, 0, 0);
    portalF->otherPortal = portalE;

    Near::input()->lockMouse(true);

    Near::FPSCounter fps;

    while(!Near::shouldClose()){
      fps.frame();
      Near::pollEvents();
      if(Near::input()->isKeyPressedThisFrame(VK_ESCAPE)) Near::markClose();
      scene.beforeUpdate(fps.getLastFrameTime());
      scene.update(fps.getLastFrameTime());
      scene.afterUpdate(fps.getLastFrameTime());
      auto* renderer = Near::renderer();
      renderer->clear(Near::Math::Color(0, 0, 0, 1));
      renderer->setWorldTransform(Near::Math::Matrix::Identity);
      scene.draw();

      std::ostringstream debugText;
      debugText << u8"FPS: " << std::fixed << std::setprecision(2) << fps.getFPS() << u8"\n";
      debugText << u8"カメラ: " << camera->transform.position.x << u8" / " << camera->transform.position.y << u8" / " << camera->transform.position.z << u8"\n";
      debugText << u8"プレイヤー: " << player->transform.position.x << u8" / " << player->transform.position.y << u8" / " << player->transform.position.z;
      renderer->drawText(debugText.str(), Near::Math::Vector2(5, 5));

      renderer->present();

      // Sleep((GetAsyncKeyState('T') & 0x8000) ? 100 : 1);
    }
  }catch(const std::exception& ex){
    Near::dialog::alert(u8"悲しいとき～", std::string(ex.what()), Near::dialog::DialogIcon::ERROR, u8"ゲームを終了");
  }

  scene.uninit();
  Near::uninit();

  return 0;
}
