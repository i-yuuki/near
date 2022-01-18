#pragma once

#include <optional>

#include "has-child.h"

namespace Near::GUI{

class Container : public HasChildren{
public:
  const std::optional<Length>& getWidth() const;
  const std::optional<Length>& getHeight() const;
  void setWidth(const std::optional<Length>& width);
  void setHeight(const std::optional<Length>& height);
  virtual void layout(const BoxConstraints& constraints) override;
protected:
  std::optional<Length> width;
  std::optional<Length> height;
};

}
