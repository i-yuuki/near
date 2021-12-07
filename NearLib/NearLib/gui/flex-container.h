#pragma once

#include "container.h"

namespace Near::GUI{


class FlexContainer : public Container{
public:
  enum class Direction{
    HORIZONTAL,
    VERTICAL,
  };
  FlexContainer(Direction direction);
  Direction getDirection() const;
  float getGap() const;
  void setDirection(Direction direction);
  void setGap(float gap);
  virtual void layout(const BoxConstraints& constraints) override;
  virtual void layoutChildren(const BoxConstraints& constraints) override;
protected:
  Direction direction;
  float gap = 0;
  // x/yサイズを主軸/交差軸サイズにします。
  Math::Vector2 getAxisSize(const Math::Vector2 size) const;
};

// TODO Componentにする ワンチャンファイル分ける
class Flexible{
public:
  Flexible(float flex, std::shared_ptr<Component> child);
  float getFlex() const;
  void setFlex(float flex);
protected:
  float flex;
  std::shared_ptr<Component> child;
};

}
