#pragma once

#include "component.h"

namespace Near::GUI{

class Container : public Component, public std::enable_shared_from_this<Container>{
public:
  void add(std::shared_ptr<Component> child);
  virtual void layout(const BoxConstraints& constraints) override;
  virtual void draw() override;
  virtual void layoutChildren(const BoxConstraints& constraints);
protected:
  std::vector<std::shared_ptr<Component>> children;
};

}
