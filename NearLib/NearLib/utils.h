#pragma once

#include <string_view>
#include <d3d11.h>

#include "near.h"

namespace Near{

void throwResult(const char* msg, HRESULT res);

template<class T>
void safeRelease(T*& thing){
  if(thing){
    thing->Release();
    thing = nullptr;
  }
}

template<class T>
void safeDestroy(T*& thing){
  if(thing){
    thing->Destroy();
    thing = nullptr;
  }
}

template<class T>
void safeUninitDelete(T*& thing){
  if(thing){
    thing->uninit();
    delete thing;
    thing = nullptr;
  }
}

template<class T>
void safeDelete(T*& thing){
  if(thing){
    delete thing;
    thing = nullptr;
  }
}

template<class T>
void safeDeleteArray(T*& thing){
  if(thing){
    delete[] thing;
    thing = nullptr;
  }
}

// 0なら0、負なら-1、正なら1を返します。
constexpr int sign(float v){
  return v > 0 ? 1 : v < 0 ? -1 : 0;
}

// ワイド文字列をUTF-8文字列に変換します。
// Windows系APIの戻り値変換にでも使ってね。
std::string narrow(std::wstring_view str);

// UTF-8文字列をワイド文字列に変換します。
// Windows系APIを呼ぶときに使ってね。
std::wstring widen(std::string_view str);

Math::Matrix createObliqueMatrix(const Math::Matrix& view, const Math::Matrix& projection, Math::Plane c);
/*
Math::Vector3 toEulerAngles(const Math::Quaternion& q);
Math::Quaternion opposite(const Math::Quaternion& q);
*/

}
