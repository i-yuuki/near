#pragma once

#include <unordered_map>

#include "shader.h"
#include "texture.h"

namespace Near::Font{

struct FontChar{
  uint32_t codepoint;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  int16_t xOffset;
  int16_t yOffset;
  int16_t xAdvance;
  uint8_t page;
  uint8_t channel;
};

class Font : public Asset{
public:
  virtual ~Font();
  virtual void load(const std::string& path) override;
private:
  int fontSize;
  int lineHeight;
  int baseline;
  std::unordered_map<uint32_t, FontChar> characters;
  std::vector<Texture> textures;
};

}
