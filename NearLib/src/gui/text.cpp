#include "pch.h"
#include "gui/text.h"

#include "near.h"

namespace Near::GUI{

Text::Text(const std::string& text, std::shared_ptr<Font::Font> font)
: Component(), text(text), font(font){
}

const std::string& Text::getText(){
  return text;
}

void Text::draw(){
  Component::draw();
  if(font == nullptr) return;
  font->drawText(text, position, Math::Vector2::Zero, 16, Math::Color(1, 1, 1, 1));
}

}
