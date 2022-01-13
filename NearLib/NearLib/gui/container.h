#pragma once

#include "has-child.h"

namespace Near::GUI{

class Container : public HasChildren{
public:
  virtual void layout(const BoxConstraints& constraints) override;
};

}
