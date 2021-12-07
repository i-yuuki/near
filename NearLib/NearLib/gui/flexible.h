#pragma once

#include "component.h"

namespace Near::GUI{

class Flexible : public Component{
public:
  Flexible(float flex, std::shared_ptr<Component> child);
  float getFlex() const;
  void setFlex(float flex);
  virtual void layout(const BoxConstraints& constraints) override;
  virtual void draw() override;
protected:
  float flex;
  std::shared_ptr<Component> child;
};

}
