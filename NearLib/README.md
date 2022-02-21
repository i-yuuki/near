# NearLib

NearLib (にあーりぶ) は Windows ゲーム開発のスタートダッシュを少しだけ助けるかもしれない DirectX 11 グラフィックライブラリです。

## 特徴

- シングルトンスタイル (glfwみたいになってもうた)
- シーン管理
- アセット管理 (テクスチャ、シェーダー、etcを同じシーンで何回も読まなくできる)
- キー & マウス入力
- オリジナル 2D GUI
- うおおおお！ぜーんぶUTF-8だ！！
- `namespace near`したかったけど`Windows.h`をインクルードすると[つらい](https://stackoverflow.com/questions/3046964/define-far-define-near-windef-h)ので`namespace Near`になった
- [手抜き™](https://github.com/microsoft/DirectXTK)

## 材料

- Windows 10 April 2018 Update 以降
- Visual Studio 2017 以降
- C++17

## 使用イメージ

空のウィンドウ

```cpp
#include <NearLib/near.h>
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
  Near::InitParams initParams;
  initParams.width = 1280;
  initParams.height = 720;
  initParams.windowTitle = u8"ほげ";
  Near::init(initParams);
  while(!Near::shouldClose()){
    Near::pollEvents();
    Near::Renderer* r = Near::renderer();
    r->clear(Near::Math::Color(0.5f, 0.5f, 0.5f, 1.0f));
    r->present();
  }
  Near::uninit();
  return 0;
}
```

シーンとデバッグオブジェクト

```cpp
#include <NearLib/near.h>
#include <NearLib/scene.h>
#include <NearLib/debug-camera.h>
#include <NearLib/debug-grid.h>
#include <NearLib/fps-counter.h>
#include <NearLib/model-object.h>

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
  Near::InitParams initParams;
  initParams.width = 1280;
  initParams.height = 720;
  initParams.windowTitle = u8"ふが";
  Near::init(initParams);

  auto scene = std::make_unique<Near::Scene>();
  scene->init();
  scene->getLayer(Near::Scene::LAYER_MANAGERS)->createGameObject<Near::DebugCamera>();
  scene->getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<Near::DebugGrid>(50);
  scene->getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<Near::ModelObject>(u8"assets/player.fbx");

  Near::FPSCounter timer;

  while(!Near::shouldClose()){
    Near::pollEvents();
    timer.frame();
    scene->update(static_cast<float>(timer.getLastFrameTime()));

    Near::Renderer* r = Near::renderer();
    r->clear(Near::Math::Color(0.5f, 0.5f, 0.5f, 1.0f));
    scene->draw();
    r->present();
  }

  scene.reset();
  Near::uninit();

  return 0;
}
```

## ドキュメント

別に用意する余裕なかったので各ヘッダーファイルにコメントで書こうとしています。
