#pragma once

#include <memory>

#include "../math.h"
#include "layout.h"

namespace Near::GUI{

enum class SizeUnit{
  PX,
  PARENT,
  FILL_CONTAINER,
};

enum class Unit{
  PX,
  PERCENT,
};

struct Length{
  float value;
  Unit unit;
  Length(float value, Unit unit);
};

class Container;
class FlexContainer;

class Component{
public:
  friend Container;
  friend FlexContainer;
  std::weak_ptr<Container> getParent() const;
  const Near::Math::Vector2& getLayoutPosition() const;
  const Near::Math::Vector2& getLayoutSize() const;
  const Near::Math::Color& getBackground() const;
  void setBackground(const Near::Math::Color& background);
  virtual void layout(const BoxConstraints& constraints);
  virtual void draw();
protected:
  std::weak_ptr<Container> parent;
  Near::Math::Vector2 layoutPosition{0, 0};
  Near::Math::Vector2 layoutSize{1, 1};
  Near::Math::Color background{0, 0, 0, 0};
};

}
