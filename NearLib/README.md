# NearLib

NearLib (にあーりぶ) は Windows ゲーム開発のスタートダッシュを少しだけ助けるかもしれない DirectX 11 グラフィックライブラリです。

## 特徴

- シングルトンスタイル (glfwみたいになってもうた)
- シーン管理
- アセット管理 (テクスチャ、シェーダー、etcを同じシーンで何回も読まなくできる)
- キー＆マウス入力 (予定)
- .wavの再生 (予定)
- DirectWriteで2D GUI (予定)
- うおおおお！ぜーんぶUTF-8だ！！
- `namespace near`したかったけど`Windows.h`をインクルードすると[つらい](https://stackoverflow.com/questions/3046964/define-far-define-near-windef-h)ので`namespace Near`になった
- [手抜き™](https://github.com/microsoft/DirectXTK)

## 材料

- Windows 10 April 2018 Update 以降
- Visual Studio 2017 以降
- C++17

## TODO

- [ ] 入力
  - キーは Raw Input
  - マウスは Raw Input (相対/移動量) & WM_MOUSEXXX (絶対位置)
  - イベント風にしたいなぁ
    - [参考](https://github.com/TheWisp/signals)
- [ ] 音
- [ ] 文字続き＆GUI
  - `IDWriteFont::GetMetrics()`

## 使用イメージ

```cpp
#include <NearLib/near.h>
#include <NearLib/scene.h>
#include <NearLib/debug-camera.h>
#include <NearLib/debug-grid.h>

class MyScene : public Near::Scene{
public:
  virtual void init() override{
    Near::Scene::init();
    getLayer(Near::Scene::LAYER_MANAGERS)->createGameObject<Near::DebugCamera>();
    getLayer(Near::Scene::LAYER_OBJECTS)->createGameObject<Near::DebugGrid>(20);
  }
};

Near::InitParams initParams;
initParams.width = 1280;
initParams.height = 720;
initParams.windowTitle = u8"モンスターハンター ライズ";
Near::init(initParams);

Near::SimpleGame game;
game.init();
game.setScene(std::make_unique<MyScene>());

while(!Near::shouldClose()){
  Near::pollEvents();
  game.update();
  game.draw();
}

game.uninit();
```

## ドキュメント

別に用意する余裕なかったので各ヘッダーファイルにコメントで書いておきました。
