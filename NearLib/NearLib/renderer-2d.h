#pragma once

#include <array>

#include "shader.h"
#include "texture.h"
#include "vertex.h"

namespace Near{

struct Vertex2D{
  Math::Vector2 position;
  Math::Color color;
  Math::Vector2 uv;
};

class Renderer2D{
public:
  Renderer2D();
  ~Renderer2D();
  void init();
  void uninit();

  bool isActive();
  void begin();
  void end();
  void flush();

  void setTexture(Texture* texture);
  void pushTransform();
  void popTransform();
  void setTransform(const Math::Matrix& transform);
  void applyTransform(const Math::Matrix& transform);
  void translate(float x, float y);
  void scale(float x, float y);
  void scale(float xy);
  void setShader(Near::PixelShader* shader);
  void resetShader();

  void fillRect(const Math::Vector2& pos, const Math::Vector2& size, const Math::Vector2& origin = Math::Vector2(0, 0), const Math::Color& color = Math::Color(1, 1, 1, 1));
  void fillRectUV(const Math::Vector2& pos, const Math::Vector2& size, const Math::Vector2& uv, const Math::Vector2& uvSize, const Math::Vector2& origin = Math::Vector2(0, 0), const Math::Color& color = Math::Color(1, 1, 1, 1));
  void fillRect(const std::array<Vertex2D, 4>& v);
private:
  static constexpr unsigned int VERTEX_BUFFER_SIZE = 1024;
  static constexpr unsigned int INDEX_BUFFER_SIZE = 1536;
  struct ConstantBufferData{
    bool useTexture;
    uint8_t _pad[16];
  };

  bool active;

  ID3D11Buffer* vertexBuffer;
  ID3D11Buffer* indexBuffer;
  ID3D11Buffer* constantBuffer;
  ConstantBufferData constantBufferData;

  Vertex3D vertices[VERTEX_BUFFER_SIZE];
  uint32_t indices[INDEX_BUFFER_SIZE];
  unsigned int vertexIdx;
  unsigned int indexIdx;
  bool createBufferSpace(unsigned int vertices, unsigned int indices);

  Texture* texture;
  Math::Matrix transform;
  std::vector<Math::Matrix> transforms;
  PixelShader* pixelShader;
  std::shared_ptr<PixelShader> defaultPixelShader;

  void addVertex(const Vertex2D& v);
  void addIndex(uint32_t idx);
};

}
