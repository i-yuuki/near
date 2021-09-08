#include "pch.h"
#include "near.h"

#include "renderer.h"
#include "renderer-2d.h"
#include "utils.h"

namespace Near{

namespace{

InputManager* g_input = nullptr;
Renderer* g_renderer = nullptr;
Renderer2D* g_renderer2D = nullptr;
bool g_closeMarked = false;

LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam){
  if(g_input->processMessage(msg, wParam, lParam)) return 0;
  switch(msg){
    // case WM_CLOSE:
    //   DestroyWindow(window);
    //   return 0;
    case WM_DESTROY:
      PostQuitMessage(0);
      markClose();
      return 0;
    default:
      return DefWindowProc(window, msg, wParam, lParam);
  }
}

}

namespace Assets{

void init();
void uninit();

}

void init(const InitParams& params){
  CoInitialize(nullptr);

  constexpr const wchar_t* windowClassName = L"NearGameWindow";
  DWORD windowStyle = WS_OVERLAPPEDWINDOW;
  HINSTANCE instance = GetModuleHandleW(nullptr);

  // おなじみ クラス登録
  WNDCLASSW cls = {};
  cls.lpfnWndProc   = WindowProc;
  cls.lpszClassName = windowClassName;
  cls.hInstance     = instance;
  cls.hCursor       = LoadCursorW(NULL, IDC_ARROW);
  cls.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // ブラシオブジェクト or (HBRUSH)(COLOR_XXX + 1)
  RegisterClassW(&cls);

  // タイトルバーとかを含めたウィンドウサイズを求める
  RECT windowRect = {0, 0, static_cast<LONG>(params.width), static_cast<LONG>(params.height)};
  AdjustWindowRect(&windowRect, windowStyle, false);
  int windowWidth  = windowRect.right - windowRect.left;
  int windowHeight = windowRect.bottom - windowRect.top;
  
  // work area (タスクバーを除いた画面サイズ) の真ん中に配置
  // デスクトップの真ん中だと画面とウィンドウのサイズが近い
  // (たとえば斡旋PCの画面1366x768 & ウィンドウ1280x720)
  // とウィンドウ下端がタスクバーに埋まるため
  RECT workRect;
  SystemParametersInfoW(SPI_GETWORKAREA, 0, &workRect, 0);
  int workWidth  = workRect.right - workRect.left;
  int workHeight = workRect.bottom - workRect.top;
  int windowX =             (workWidth  - windowWidth)  / 2;
  int windowY = std::max(0, (workHeight - windowHeight) / 2);

  HWND window = CreateWindowW(
    windowClassName,
    widen(params.windowTitle).c_str(),
    windowStyle,
    windowX,
    windowY,
    windowWidth,
    windowHeight,
    NULL,
    NULL,
    instance,
    NULL
  );
  
  if(!window){
    throw std::exception("CreateWindow failed");
  }
  
  ShowWindow(window, SW_NORMAL);
  UpdateWindow(window);

  Assets::init();

  g_input = new InputManager();
  g_input->init(window);

  g_renderer = new Renderer();
  g_renderer->init(window, params.width, params.height);

  g_renderer2D = new Renderer2D();
  g_renderer2D->init();

  g_closeMarked = false;
}

void uninit(){
  DestroyWindow(g_renderer->getWindow());
  safeUninitDelete(g_input);
  safeUninitDelete(g_renderer2D);
  safeUninitDelete(g_renderer);
  Assets::uninit();
  CoUninitialize();
}

void pollEvents(){
  if(g_input){
    g_input->beforePollEvents();
  }
  MSG msg = {};
  while(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)){
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
}

void markClose(){
  g_closeMarked = true;
}

bool shouldClose(){
  return g_closeMarked;
}

Renderer* renderer(){
  return g_renderer;
}

Renderer2D* renderer2D(){
  return g_renderer2D;
}

InputManager* input(){
  return g_input;
}

}
