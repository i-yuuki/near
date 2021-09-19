#pragma once

#include <memory>

#include "../math.h"

namespace Near::GUI{

/*
enum class SizeUnit{
  PX,
  PERCENT,
  FILL_CONTAINER,
};
*/

class Component{
public:
  std::weak_ptr<Component> getParent();
  const Near::Math::Vector2& getPosition();
  const Near::Math::Vector2& getSize();
  const Near::Math::Color& getBackground();
  void setPosition(const Near::Math::Vector2& position);
  void setSize(const Near::Math::Vector2& size);
  void setBackground(const Near::Math::Color& background);
  virtual void draw();
protected:
  std::weak_ptr<Component> parent;
  Near::Math::Vector2 position{0, 0};
  Near::Math::Vector2 size{1, 1};
  Near::Math::Color background;
  /*
  SizeUnit widthUnit = SizeUnit::PX;
  SizeUnit heightUnit = SizeUnit::PX;
  */
};

}
