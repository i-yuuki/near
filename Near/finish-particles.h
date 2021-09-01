#pragma once

#include <NearLib/game-object.h>
#include <NearLib/buffer.h>

class FinishParticles : public Near::GameObject{
public:
  virtual void init(Near::Layer* layer) override;
  virtual void update(float deltaTime) override;
  virtual void draw() override;
  virtual void uninit() override;
private:
  struct Particle{
    Near::Math::Vector3 position;
    Near::Math::Vector3 velocity;
    float time;
    float lifetime;
  };
  float time = 0;
  std::vector<Particle> particles;
  Near::StandardVertexBuffer vertexBuffer;
  std::shared_ptr<Near::Texture> texture;
  std::shared_ptr<Near::VertexShader> vertexShader;
  std::shared_ptr<Near::PixelShader> pixelShader;
  void addParticle();
};
