#pragma once

#include "component.h"

namespace Near::GUI{

class Text : public Component{
public:
  Text(const std::string& text);
  const std::string& getText();
  virtual void draw() override;
private:
  std::string text;
};

}
