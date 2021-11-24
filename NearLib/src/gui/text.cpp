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

void Text::draw(){
  Component::draw();
  if(font == nullptr) return;
  font->drawText(text, layoutPosition, Math::Vector2::Zero, fontSize, Math::Color(1, 1, 1, 1));
}

}
