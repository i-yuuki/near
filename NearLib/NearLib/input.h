#pragma once

#include <unordered_set>

#include <Windows.h>

#include "event.h"

namespace Near{

enum class ButtonState{
  NOT_PRESSED,
  PRESSED,
  DOWN,
  RELEASED,
};

// キーとマウスの入力を管理します。
// ついでにSignal-Slotの実験をしています (非スレッドセーフ)
class InputManager{
public:
  struct KeyEvent{
    int vkey;
    bool isRepeat;
  };
  struct MouseEvent{
    // ボタン番号
    // (0 = 左クリック, 1 = 右クリック, etc)
    int button;
    int x;
    int y;
  };
  void init(HWND window);
  void uninit();
  // キーが押されているかを返します。
  // @param key 仮想キーコード
  // @returns 押されていたらtrue
  bool isKeyDown(int key);
  // 現在のフレームでキーがちょうど押されたかを返します。
  // Near::pollEvents()が低頻度で呼ばれていると、
  // キーが連打されたときに実際より少ない回数しか検出できないため、
  // キーイベントハンドラーを使うことも検討してください。
  // @param key 仮想キーコード
  bool isKeyPressedThisFrame(int key);
  int getMouseX();
  int getMouseY();
  int getMouseMovementX();
  int getMouseMovementY();
  bool isMouseLocked();
  void lockMouse(bool lock);
  // NearLib内部で使われます。
  void beforePollEvents();
  // NearLib内部で使われます。
  // void afterPollEvents();
  // ウィンドウメッセージを見ます。NearLib内部で使われます。
  // @returns メッセージが処理されたかどうか (入力と関係ないウィンドウメッセージだったらfalse)
  bool processMessage(UINT message, WPARAM wParam, LPARAM lParam);
  Event::Signal<KeyEvent> onKeyDown;
  Event::Signal<KeyEvent> onKeyUp;
  Event::Signal<MouseEvent> onMouseDown;
  Event::Signal<MouseEvent> onMouseUp;
private:
  HWND window;
  
  constexpr static int BUTTON_COUNT = 256;
  ButtonState buttons[BUTTON_COUNT];

  int mouseX;
  int mouseY;
  int mouseMovementX;
  int mouseMovementY;
  bool mouseLocked;
};

}
