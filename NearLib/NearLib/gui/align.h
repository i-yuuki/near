#pragma once

#include "has-child.h"

namespace Near::GUI{

class Align : public HasChild{
public:
  Align(const Math::Vector2& align);
  virtual void layout(const BoxConstraints& constraints) override;
  static std::shared_ptr<Align> Create(std::shared_ptr<Component> child, const Math::Vector2& align);
private:
  Math::Vector2 align;
};

}
