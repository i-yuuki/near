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
  virtual void layout() override;
protected:
  Direction direction;
  // x/yサイズを主軸/交差軸サイズにします。
  Math::Vector2 getAxisSize(const Math::Vector2 size);
};

}
