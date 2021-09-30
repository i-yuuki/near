#pragma once

#include "component.h"
#include "../font.h"

namespace Near::GUI{

class Text : public Component{
public:
  Text(const std::string& text, std::shared_ptr<Font::Font> font);
  const std::string& getText() const;
  virtual void draw() override;
private:
  std::string text;
  std::shared_ptr<Font::Font> font;
};

}
