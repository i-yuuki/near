#pragma once

#include "component.h"

namespace Near::GUI{

class HasChild : public Component, public std::enable_shared_from_this<HasChild>{
public:
  void setChild(std::shared_ptr<Component> child);
  virtual Component* getDeepComponentAt(const Math::Vector2& point) override;
  virtual void draw() override;
protected:
  std::shared_ptr<Component> child;
};

class HasChildren : public Component, public std::enable_shared_from_this<HasChildren>{
public:
  void add(std::shared_ptr<Component> child);
  void remove(std::shared_ptr<Component> child);
  virtual Component* getDeepComponentAt(const Math::Vector2& point) override;
  virtual void draw() override;
protected:
  std::vector<std::shared_ptr<Component>> children;
};

}
