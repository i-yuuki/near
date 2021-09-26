#pragma once

#include "container.h"

namespace Near::GUI{


class FlexContainer : public TContainer<FlexContainer>{
public:
  enum class Direction{
    HORIZONTAL,
    VERTICAL,
  };
  FlexContainer(Direction direction);
  Direction getDirection();
  void setDirection(Direction direction);
  void layout();
protected:
  Direction direction;
  // x/yサイズを主軸/交差軸サイズにします。
  Math::Vector2 getAxisSize(const Math::Vector2 size);
};

}
