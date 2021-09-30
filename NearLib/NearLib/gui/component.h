#pragma once

#include <memory>

#include "../math.h"

namespace Near::GUI{

enum class SizeUnit{
  PX,
  PARENT,
  FILL_CONTAINER,
};

class Container;
class FlexContainer;

class Component{
public:
  friend Container;
  friend FlexContainer;
  std::weak_ptr<Container> getParent() const;
  const Near::Math::Vector2& getPosition() const;
  const Near::Math::Vector2& getSize() const;
  const Near::Math::Vector2& getLayoutSize() const;
  SizeUnit getWidthUnit() const;
  SizeUnit getHeightUnit() const;
  const Near::Math::Color& getBackground() const;
  void setPosition(const Near::Math::Vector2& position);
  void setSize(const Near::Math::Vector2& size);
  void setWidthUnit(SizeUnit unit);
  void setHeightUnit(SizeUnit unit);
  void setBackground(const Near::Math::Color& background);
  virtual void draw();
protected:
  std::weak_ptr<Container> parent;
  Near::Math::Vector2 position{0, 0};
  Near::Math::Vector2 size{1, 1};
  Near::Math::Vector2 layoutSize{1, 1};
  Near::Math::Color background;
  SizeUnit widthUnit = SizeUnit::PX;
  SizeUnit heightUnit = SizeUnit::PX;
 virtual void sizeChanged();
};

}
