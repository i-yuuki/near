#include "pch.h"
#include "gui/text.h"

#include "near.h"

namespace Near::GUI{

Text::Text(const std::string& text, std::shared_ptr<Font::Font> font)
: Component(), text(text), font(font), fontSize(16){
}

const std::string& Text::getText() const{
  return text;
}

float Text::getFontSize() const{
  return fontSize;
}

void Text::setFontSize(float fontSize){
  this->fontSize = fontSize;
}

const Math::Vector2& Text::getTextAlign() const{
  return textAlign;
}

void Text::setTextAlign(const Math::Vector2& textAlign){
  this->textAlign = textAlign;
}

const Math::Color& Text::getForeground() const{
  return foreground;
}

void Text::setForeground(const Math::Color& foreground){
  this->foreground = foreground;
}

void Text::layout(const BoxConstraints& constraints){
  auto textSize = font->measureText(text, fontSize);
  layoutSize.x = std::max(textSize.x, constraints.minWidth);
  layoutSize.y = std::max(textSize.y, constraints.minHeight);
}

void Text::draw(){
  Component::draw();
  if(font == nullptr) return;
  font->drawText(text, layoutPosition + layoutSize * textAlign, textAlign, fontSize, foreground);
}

}
