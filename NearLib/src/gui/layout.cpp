#include "pch.h"
#include "gui/layout.h"

namespace Near::GUI{

BoxConstraints::BoxConstraints(){
}

BoxConstraints::BoxConstraints(float minWidth, float minHeight, float maxWidth, float maxHeight) : minWidth(minWidth), minHeight(minHeight), maxWidth(maxWidth), maxHeight(maxHeight){
}

BoxConstraints::BoxConstraints(const Math::Vector2& minSize, const Math::Vector2& maxSize) : minWidth(minSize.x), minHeight(minSize.y), maxWidth(maxSize.x), maxHeight(maxSize.y){
}

BoxConstraints& BoxConstraints::setWidth(float width){
  minWidth = width;
  maxWidth = width;
  return *this;
}

BoxConstraints& BoxConstraints::setHeight(float height){
  minHeight = height;
  maxWidth = height;
  return *this;
}

BoxConstraints& BoxConstraints::setMinWidth(float width){
  minWidth = width;
  return *this;
}

BoxConstraints& BoxConstraints::setMinHeight(float height){
  minHeight = height;
  return *this;
}

BoxConstraints& BoxConstraints::setMaxWidth(float width){
  maxWidth = width;
  return *this;
}

BoxConstraints& BoxConstraints::setMaxHeight(float height){
  maxHeight = height;
  return *this;
}


}
