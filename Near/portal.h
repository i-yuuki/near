#pragma once

#include <NearLib/game-object.h>
#include <NearLib/buffer.h>
#include <NearLib/shader.h>

class Portal : public Near::GameObject{
public:
  std::weak_ptr<Portal> otherPortal;
  Portal(Near::Math::Vector2 extents, Near::Math::Color color = Near::Math::Color(1, 1, 1, 1));
  virtual void init(Near::Layer* layer) override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  const Near::Math::Vector2& getExtents() const;
  bool intersects(const Near::Math::Vector3& origin, const Near::Math::Vector3& ray, float* dist);
  // void drawMask();
protected:
  Near::Math::Vector2 extents;
  Near::Math::Color color;
  Near::Texture texture;
  Near::Texture texture1;
  // Near::Texture textureMask;
  Near::StandardVertexBuffer vertexBuffer;
  std::shared_ptr<Near::VertexShader> vertexShader;
  std::shared_ptr<Near::PixelShader> pixelShader;
};
