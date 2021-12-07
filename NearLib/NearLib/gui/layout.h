#pragma once

#include "../math.h"

namespace Near::GUI{

struct BoxConstraints{
  float minWidth = 0;
  float minHeight = 0;
  float maxWidth = std::numeric_limits<float>::infinity();
  float maxHeight = std::numeric_limits<float>::infinity();
  BoxConstraints();
  BoxConstraints(float minWidth, float minHeight, float maxWidth, float maxHeight);
  BoxConstraints(const Math::Vector2& minSize, const Math::Vector2& maxSize);
  BoxConstraints& setWidth(float width);
  BoxConstraints& setHeight(float height);
  BoxConstraints& setMinWidth(float width);
  BoxConstraints& setMinHeight(float height);
  BoxConstraints& setMaxWidth(float width);
  BoxConstraints& setMaxHeight(float height);
};

}
