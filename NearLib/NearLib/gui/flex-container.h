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
  virtual void layout() override;
  virtual void layoutChildren() override;
protected:
  Direction direction;
  float gap = 0;
  // x/yサイズを主軸/交差軸サイズにします。
  Math::Vector2 getAxisSize(const Math::Vector2 size) const;
  SizeUnit getMainAxisUnit(Component* comp) const;
  SizeUnit getCrossAxisUnit(Component* comp) const;
};

}
