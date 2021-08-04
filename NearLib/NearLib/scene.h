#pragma once

#include <map>

#include "near.h"
#include "layer.h"
#include "assets.h"
#include "shader.h"
#include "texture.h"

namespace Near{

class Scene{
public:
  static constexpr int LAYER_MANAGERS = 0;
  static constexpr int LAYER_OBJECTS = 100;
  static constexpr int LAYER_TRANSPARENT_OBJECTS = 200;
  static constexpr int LAYER_OVERLAY = 300;
  Scene();
  virtual ~Scene();
  virtual void init();
  virtual void beforeUpdate(float deltaTime);
  virtual void update(float deltaTime);
  virtual void afterUpdate(float deltaTime);
  virtual void draw();
  virtual void uninit();
  // レイヤー番号からレイヤーを返します。
  // @return レイヤー。なかったら`nullptr`
  Layer* getLayer(int num);
  // この番号のレイヤーがなければ作ります。
  // @return 作られたレイヤー。レイヤーが既に存在していたらそれ。
  Layer* insertLayer(int num);
  // この番号のレイヤーがあれば、その中のすべてのオブジェクトとともに削除します。
  void deleteLayer(int num);
  // 同じクラスかサブクラスのインスタンスをすべてのレイヤーから探します。
  // サブクラスのインスタンスを無視できる場合、より高速に動作する`findObjectOfExactType()`がおすすめです。
  // @return 見つかった最初のオブジェクト。なかったら`nullptr`
  template<class T>
  std::shared_ptr<T> findObjectOfType(){
    for(auto& it : layers){
      if(std::shared_ptr<T> obj = it.second.findObjectOfType<T>()) return obj;
    }
    return nullptr;
  }
  // 同じクラスのインスタンスをすべてのレイヤーから探します。
  // サブクラスのインスタンスを無視する分高速に動作します。
  // @return 見つかった最初のオブジェクト。なかったら`nullptr`
  template<class T>
  std::shared_ptr<T> findObjectOfExactType(){
    for(auto& it : layers){
      if(std::shared_ptr<T> obj = it.second.findObjectOfExactType<T>()) return obj;
    }
    return nullptr;
  }
  // 同じクラスかサブクラスのインスタンスをすべてのレイヤーから探してリストに追加します。
  // リストに既に入っているオブジェクトはそのまま残ります。自身でクリアしてください。
  // サブクラスのインスタンスを無視できる場合、より高速に動作する`findObjectOfExactType()`がおすすめです。
  // @param out 見つかったオブジェクトが追加されるリスト
  // @return 見つかったオブジェクトの個数
  template<class T>
  int findObjectsOfType(std::vector<std::shared_ptr<T>>& out){
    int total = 0;
    for(auto& it : layers){
      total += it.second.findObjectsOfType(out);
    }
    return total;
  }
  // 同じクラスのインスタンスをすべてのレイヤーから探してリストに追加します。
  // リストに既に入っているオブジェクトはそのまま残ります。自身でクリアしてください。
  // サブクラスのインスタンスを無視する分高速に動作します。
  // @param out 見つかったオブジェクトが追加されるリスト
  // @return 見つかったオブジェクトの個数
  template<class T>
  int findObjectsOfExactType(std::vector<std::shared_ptr<T>>& out){
    int total = 0;
    for(auto& it : layers){
      total += it.second.findObjectsOfExactType(out);
    }
    return total;
  }
  AssetManager<VertexShader>* vertexShaders;
  AssetManager<PixelShader>* pixelShaders;
  AssetManager<Texture>* textures;
protected:
  std::map<int, Layer> layers;
};

}
