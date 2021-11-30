#pragma once

#include "component.h"
#include "../font.h"

namespace Near::GUI{

class Text : public Component{
public:
  Text(const std::string& text, std::shared_ptr<Font::Font> font);
  const std::string& getText() const;
  float getFontSize() const;
  void setFontSize(float fontSize);
  const Math::Vector2& getTextAlign() const;
  void setTextAlign(const Math::Vector2& textAlign);
  const Math::Color& getForeground() const;
  void setForeground(const Math::Color& foreground);
  virtual void draw() override;
private:
  std::string text;
  std::shared_ptr<Font::Font> font;
  float fontSize;
  Math::Vector2 textAlign;
  Math::Color foreground{1, 1, 1, 1};
};

}
