#pragma once

#include <NearLib/near.h>
#include <NearLib/buffer.h>
#include <NearLib/game-object.h>

class Polygon2D : public Near::GameObject{
public:
  Polygon2D(const std::string& path, Near::Math::Vector2 position, Near::Math::Vector2 size);
  virtual void init(Near::Layer* layer) override;
  virtual void draw() override;
  virtual void uninit() override;
  void setColor(const Near::Math::Color& color);
private:
  std::string path;
  Near::Math::Vector2 position;
  Near::Math::Vector2 size;
  Near::Vertex3D vertices[4];
  Near::StandardVertexBuffer vertexBuffer;
  std::shared_ptr<Near::Texture> texture;
  std::shared_ptr<Near::VertexShader> vertexShader;
  std::shared_ptr<Near::PixelShader> pixelShader;
};
