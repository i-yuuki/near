#pragma once

#include "has-child.h"

namespace Near::GUI{

class FlexContainer : public HasChildren{
public:
  enum class Direction{
    HORIZONTAL,
    VERTICAL,
  };
  enum class CrossAxisAlign{
    START,
    CENTER,
    END,
    STRETCH,
  };
  FlexContainer(Direction direction);
  Direction getDirection() const;
  CrossAxisAlign getCrossAxisAlign() const;
  float getGap() const;
  void setDirection(Direction direction);
  void setCrossAxisAlign(CrossAxisAlign align);
  void setGap(float gap);
  virtual void layout(const BoxConstraints& constraints) override;
protected:
  Direction direction;
  CrossAxisAlign crossAxisAlign = CrossAxisAlign::START;
  float gap = 0;
  // x/yサイズを主軸/交差軸サイズにします。
  Math::Vector2 getAxisSize(const Math::Vector2 size) const;
};

}
