#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include "DirectXTK/SimpleMath.h"
#include "renderer.h"
#include "input.h"
#include "assets.h"

namespace Near{

namespace Math = DirectX::SimpleMath;

class Renderer;

struct InitParams{
  // GPUの選び方を設定出来たら面白いねっていうアイデアを書いてみただけのもの
  enum class Adapter{
    // 認識する最初のアダプターを選びます。
    FIRST,
    // 最もパフォーマンスの高いハードウェアアダプターを選びます。
    HIGH_PERFORMANCE,
    // 最もエネルギーを節約できるアダプターを選びます。
    LOW_POWER,
  };
  int width = 1280;
  int height = 720;
  std::string windowTitle;
  Adapter adapter = Adapter::HIGH_PERFORMANCE; // Not used yet
};

// NearLibを初期化し、ゲームウィンドウを表示します。
// プログラムの最初で1回呼ぶ必要があります。
void init(const InitParams& params);

// ゲームウィンドウを閉じてNearLibを解放します。
// プログラムの最後で1回呼ぶ必要があります。
void uninit();

// ウィンドウイベントを読み取ります。
// 毎フレーム、ゲームの更新前に呼ぶべきです。
void pollEvents();

// NearLibに終了を合図します。
// これを呼んでからは、shouldClose()がtrueを返します。
void markClose();

// markClose()でNearLibの終了が合図されているかを返します。
// ゲームループの条件式にでもしてください。
bool shouldClose();

InputManager* input();

// Nearレンダラーを返します。
Renderer* renderer();

}
