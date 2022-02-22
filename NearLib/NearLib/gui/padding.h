#pragma once

#include "has-child.h"

namespace Near::GUI{

class Padding : public HasChild{
public:
  Padding(float vertical, float horizontal);
  Padding(float top, float right, float bottom, float left);
  virtual void layout(const BoxConstraints& constraints) override;
  template<typename... Args>
  static std::shared_ptr<Padding> Create(std::shared_ptr<Component> child, Args&&... args){
    auto me = std::make_shared<Padding>(std::forward<Args>(args)...);
    me->setChild(child);
    return me;
  }
private:
  float top;
  float right;
  float bottom;
  float left;
};

}
