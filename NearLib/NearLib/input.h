#pragma once

#include <unordered_set>

#include <Windows.h>

namespace Near{

enum class ButtonState{
  NOT_PRESSED,
  PRESSED,
  DOWN,
  RELEASED,
};

class AInputListener{};

class KeyListener : public AInputListener{
public:
  virtual void onKeyDown(int key, bool isRepeat){}
  virtual void onKeyUp(int key){}
};

// InputManagerとそこに追加された入力リスナーをつなぐオブジェクトです。
// リスナーが呼ばれてほしい間、変数にとっておいてください。
// disconnect()またはデストラクターが呼ばれるとリスナーが削除されます。
// @example
// class Player : public Near::GameObject, public Near::KeyListener{
// public:
//   virtual void init(Near::Layer* layer) override{
//     Near::GameObject::init(layer);
//     keyListener = Near::input()->addKeyListener(this);
//   }
//   virtual void uninit() override{
//     keyListener.disconnect();
//   }
//   virtual void onKeyDown(int vkey, bool repeat) override{
//     if(vkey == VK_SPACE && !repeat){
//       jump();
//     }
//   }
// private:
//   Near::InputConnection keyListener;
// };
class InputConnection{
public:
  void* signal = nullptr;
  void* listener = nullptr;
  InputConnection() = default;
  InputConnection(void* signal, void* listener);
  InputConnection(const InputConnection& copyFrom) = delete;
  InputConnection& operator=(const InputConnection&) = delete;
  InputConnection& operator=(InputConnection&& moveFrom) noexcept;
  ~InputConnection();
  void disconnect();
};

// キーとマウスの入力を管理します。
// ついでにSignal-Slotの実験をしています (非スレッドセーフ)
class InputManager{
public:
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
  // キーが押されたとき・離されたときに呼ばれる、キーリスナーを追加します。
  // 戻り値の詳しい説明はInputConnectionクラスのコメントに。
  InputConnection addKeyListener(KeyListener* listener);
  // NearLib内部で使われます。
  void beforePollEvents();
  // NearLib内部で使われます。
  // void afterPollEvents();
  // ウィンドウメッセージを見ます。NearLib内部で使われます。
  // @returns メッセージが処理されたかどうか (入力と関係ないウィンドウメッセージだったらfalse)
  bool processMessage(UINT message, WPARAM wParam, LPARAM lParam);
private:
  HWND window;
  
  constexpr static int BUTTON_COUNT = 256;
  ButtonState buttons[BUTTON_COUNT];

  int mouseX;
  int mouseY;
  int mouseMovementX;
  int mouseMovementY;
  bool mouseLocked;

  std::unordered_set<KeyListener*> keyListeners;
};

}
