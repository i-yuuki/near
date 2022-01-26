#pragma once

#include "../math.h"

namespace Near::GUI{

class Component;

class Event{
public:
  Event(Component* source);
  Component* getSource();
private:
  Component* source;
};

class MouseEvent : public Event{
public:
  MouseEvent(Component* source, const Math::Vector2& position, int button);
  const Math::Vector2& getPosition() const;
  int getButton() const;
private:
  Math::Vector2 position;
  int button;
};

}
