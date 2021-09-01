#include "finish-particles.h"

#include <random>

#include <NearLib/camera.h>
#include <NearLib/scene.h>

void FinishParticles::init(Near::Layer* layer){
  GameObject::init(layer);
  time = 0;
  particles.clear();
  Near::Vertex3D vertices[4] = {
    { Near::Math::Vector3(-3.0f,  3.0f, 0), Near::Math::Vector3(0, 0, -1), Near::Math::Color(0.157f, 0.804f, 0.255f, 1), Near::Math::Vector2(0, 0) },
    { Near::Math::Vector3( 3.0f,  3.0f, 0), Near::Math::Vector3(0, 0, -1), Near::Math::Color(0.157f, 0.804f, 0.255f, 1), Near::Math::Vector2(1, 0) },
    { Near::Math::Vector3(-3.0f, -3.0f, 0), Near::Math::Vector3(0, 0, -1), Near::Math::Color(0.157f, 0.804f, 0.255f, 1), Near::Math::Vector2(0, 1) },
    { Near::Math::Vector3( 3.0f, -3.0f, 0), Near::Math::Vector3(0, 0, -1), Near::Math::Color(0.157f, 0.804f, 0.255f, 1), Near::Math::Vector2(1, 1) },
  };
  vertexBuffer.init(false, 4, vertices);
  texture = layer->getScene()->textures->getOrLoad("assets/textures/finish-particle.png");
  vertexShader = layer->getScene()->vertexShaders->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = layer->getScene()->pixelShaders->getOrLoad("assets/nearlib/shaders/ps.hlsl");
}

void FinishParticles::update(float deltaTime){
  time += deltaTime;
  while(time >= 30){
    addParticle();
    time -= 30;
  }
  for(auto& p : particles){
    p.time += deltaTime;
    p.position += p.velocity * (deltaTime * 0.001f);
  }
  particles.erase(std::remove_if(particles.begin(), particles.end(), [](auto& p){ return p.time >= p.lifetime; }), particles.end());
}

void FinishParticles::draw(){
  auto cam = getLayer()->getScene()->findObjectOfType<Near::Camera>();
  if(!cam) return;
  auto camUp = cam->transform.getUp();
  auto camForward = cam->transform.getForward();
  auto* r = Near::renderer();
  auto view = r->getViewTransform().Invert();
  view.m[3][0] = 0;
  view.m[3][1] = 0;
  view.m[3][2] = 0;
  r->setVertexShader(vertexShader.get());
  r->setPixelShader(pixelShader.get());
  r->setTexture(texture.get());
  r->pushWorldTransform();
  for(auto& p : particles){
    r->setWorldTransform(Near::Math::Matrix::CreateBillboard(p.position, cam->transform.position, camUp, &camForward));
    vertexBuffer.draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  }
  r->popWorldTransform();
}

void FinishParticles::uninit(){
  GameObject::uninit();
}

void FinishParticles::addParticle(){
  std::random_device seed;
  std::mt19937 rand(seed());
  std::uniform_real_distribution<float> dist01(0, 1);
  std::uniform_real_distribution<float> dist11(-1, 1);
  particles.push_back({
    transform.position + Near::Math::Vector3(
      dist11(rand) * 32,
      dist01(rand) * 64,
      dist11(rand) * 32
    ),
    Near::Math::Vector3(
      dist11(rand) * 8,
      dist01(rand) * 32,
      dist11(rand) * 8
    ),
    0,
    1000 + dist11(rand) * 200
  });
}
