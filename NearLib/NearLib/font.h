#pragma once

#include <unordered_map>

#include "buffer.h"
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

struct LineMetrics{
  uint32_t width;
  const std::string_view text;
};

class Font : public Asset{
public:
  virtual ~Font();
  virtual void load(const std::string& path) override;
  void drawText(const std::string_view text, const Math::Vector2& position, const Math::Vector2& origin, float size, const Math::Color& color);
private:
  static constexpr unsigned int VERTEX_BUFFER_RECTS = 1024;
  int fontSize;
  int lineHeight;
  int baseline;
  int textureWidth;
  int textureHeight;
  std::unordered_map<uint32_t, FontChar> characters;
  std::vector<Texture> textures;
  StandardVertexBuffer vertexBuffer;
  IndexBuffer indexBuffer;
  Vertex3D vertices[VERTEX_BUFFER_RECTS * 4];
  unsigned int rectIdx;
  Texture* texture;
  FontChar* findChar(uint32_t codepoint);
  void calcLineWidths(const std::string_view text, std::vector<LineMetrics>& out);
  void addRect(Texture* texture, float x, float y, float w, float h, const Math::Color& color);
  void addChar(const FontChar* ch, const Math::Vector2& pos, const Math::Color& color);
  void flush();
};

}
