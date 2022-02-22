#pragma once

#include <memory>

#include "../event.h"
#include "../math.h"
#include "event.h"
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
class HasChild;
class HasChildren;
class Padding;
class FlexContainer;

class Component{
public:
  friend Container;
  friend HasChild;
  friend HasChildren;
  friend Padding;
  friend FlexContainer;
  std::weak_ptr<Component> getParent() const;
  // このコンポーネントに点が含まれているか調べます。
  bool contains(const Math::Vector2& point);
  // このコンポーネントまたは子に点が含まれているか探します。
  // @returns 点を含む最後の最も深いコンポーネント、なければnullptr
  virtual Component* getDeepComponentAt(const Math::Vector2& point);
  const Near::Math::Vector2& getLayoutPosition() const;
  const Near::Math::Vector2& getLayoutSize() const;
  const Near::Math::Color& getBackground() const;
  void setBackground(const Near::Math::Color& background);
  void propagateMouseDownEvent(MouseEvent e);
  virtual void layout(const BoxConstraints& constraints);
  virtual void draw();
  Near::Event::Signal<MouseEvent> onMouseDown;
protected:
  std::weak_ptr<Component> parent;
  Near::Math::Vector2 layoutPosition{0, 0};
  Near::Math::Vector2 layoutSize{1, 1};
  Near::Math::Color background{0, 0, 0, 0};
};

}
