#pragma once

#include "near.h"
#include "transform.h"

namespace Near{

class Layer;

class GameObject{
public:
  GameObject();
  virtual void init(Layer* layer);
  virtual void beforeUpdate(float deltaTime);
  virtual void update(float deltaTime);
  virtual void afterUpdate(float deltaTime);
  virtual void draw();
  virtual void uninit();
  Layer* getLayer();
  bool isRemoveMarked();
  // このオブジェクトを持つレイヤーのupdateのあと、afterUpdateの前で、
  // このオブジェクトがレイヤーから削除されるようにします。
  void markRemove();
  Transform transform;
protected:
  // サブクラスが init() override とかで super::init(...) できるように by Javaオタク
  using super = GameObject;
private:
  Layer* layer;
  bool removeMarked = false;
};

}
