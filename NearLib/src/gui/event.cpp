#include "pch.h"
#include "gui/event.h"

namespace Near::GUI{

Event::Event(Component* source) : source(source){
}

Component* Event::getSource(){
  return source;
}



MouseEvent::MouseEvent(Component* source, const Math::Vector2& position, int button)
  : Event(source), position(position), button(button){
}

const Math::Vector2& MouseEvent::getPosition() const{
  return position;
}

int MouseEvent::getButton() const{
  return button;
}

}
