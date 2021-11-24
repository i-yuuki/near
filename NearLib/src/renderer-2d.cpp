#include "pch.h"
#include "renderer-2d.h"

#include "utils.h"

namespace Near{

Renderer2D::Renderer2D(){
}

Renderer2D::~Renderer2D(){
  uninit();
}

void Renderer2D::init(){
  HRESULT res;
  auto* device = renderer()->getDevice();

  active = false;

  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  bufferDesc.ByteWidth = sizeof(vertices);
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  res = device->CreateBuffer(&bufferDesc, nullptr, &vertexBuffer);
  if(FAILED(res)) throwResult("CreateBuffer failed", res);

  bufferDesc.ByteWidth = sizeof(indices);
  bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  res = device->CreateBuffer(&bufferDesc, nullptr, &indexBuffer);
  if(FAILED(res)) throwResult("CreateBuffer failed", res);

  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(constantBufferData) / 16 * 16;
  bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  res = device->CreateBuffer(&bufferDesc, nullptr, &constantBuffer);
  if(FAILED(res)) throwResult("CreateBuffer failed", res);

  for(unsigned int i = 0;i < VERTEX_BUFFER_SIZE;i ++){
    vertices[i] = {Math::Vector3::Zero, Math::Vector3::Forward, Math::Vector4::One, Math::Vector2::Zero};
  }

  pixelShader = Assets::pixelShaders()->getOrLoad("assets/nearlib/shaders/ps-sprite.hlsl");
}

void Renderer2D::uninit(){
  pixelShader.reset();
  safeRelease(indexBuffer);
  safeRelease(vertexBuffer);
}

bool Renderer2D::isActive(){
  return active;
}

void Renderer2D::begin(){
  active = true;
  vertexIdx = 0;
  indexIdx = 0;
  texture = nullptr;
  auto* r = renderer();
  r->setPixelShader(pixelShader.get());
  r->getDeviceContext()->PSSetConstantBuffers(0, 1, &constantBuffer);
  r->setViewTransform(Near::Math::Matrix::Identity);
  r->setProjectionTransform(Near::Math::Matrix::CreateOrthographicOffCenter(0, r->getWidth(), r->getHeight(), 0, 0, 1));
  transforms.clear();
  setTransform(Math::Matrix::Identity);
}

void Renderer2D::end(){
  flush();
  active = false;
}

void Renderer2D::flush(){
  if(vertexIdx == 0 && indexIdx == 0) return;

  auto* r = renderer();
  auto* ctx = r->getDeviceContext();
  HRESULT res;

  D3D11_MAPPED_SUBRESOURCE resource;
  res = ctx->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
  if(FAILED(res)) throwResult("Renderer2D vertex buffer Map failed", res);
  std::memcpy(resource.pData, vertices, sizeof(Vertex3D) * vertexIdx);
  ctx->Unmap(vertexBuffer, 0);

  res = ctx->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
  if(FAILED(res)) throwResult("Renderer2D index buffer Map failed", res);
  std::memcpy(resource.pData, indices, sizeof(int32_t) * indexIdx);
  ctx->Unmap(indexBuffer, 0);

  r->setTexture(texture);
  UINT stride = sizeof(Vertex3D);
  UINT offset = 0;
  ctx->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
  ctx->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
  ctx->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  constantBufferData.useTexture = texture;
  ctx->UpdateSubresource(constantBuffer, 0, nullptr, &constantBufferData, 0, 0);
  ctx->DrawIndexed(indexIdx, 0, 0);

  vertexIdx = 0;
  indexIdx = 0;
}

void Renderer2D::setTexture(Texture* texture){
  if(this->texture == texture) return;
  flush();
  this->texture = texture;
}

void Renderer2D::pushTransform(){
  transforms.push_back(transform);
}

void Renderer2D::popTransform(){
  if(transforms.empty()) return;
  setTransform(*transforms.rbegin());
  transforms.pop_back();
}

void Renderer2D::setTransform(const Math::Matrix& transform){
  flush();
  this->transform = transform;
  renderer()->setWorldTransform(transform);
}

void Renderer2D::applyTransform(const Math::Matrix& transform){
  setTransform(transform * this->transform);
}

void Renderer2D::translate(float x, float y){
  applyTransform(Math::Matrix::CreateTranslation(x, y, 0));
}

void Renderer2D::scale(float x, float y){
  applyTransform(Math::Matrix::CreateScale(x, y, 1));
}

void Renderer2D::scale(float xy){
  applyTransform(Math::Matrix::CreateScale(xy, xy, 1));
}

void Renderer2D::fillRect(const Math::Vector2& pos, const Math::Vector2& size, const Math::Vector2& origin, const Math::Color& color){
  fillRectUV(pos, size, Math::Vector2(0, 0), Math::Vector2(1, 1), origin, color);
}

void Renderer2D::fillRectUV(const Math::Vector2& pos, const Math::Vector2& size, const Math::Vector2& uv, const Math::Vector2& uvSize, const Math::Vector2& origin, const Math::Color& color){
  createBufferSpace(4, 6);
  auto topleft = pos - size * origin;
  auto idx = vertexIdx;
  fillRect({{
    {Math::Vector2(topleft.x,          topleft.y + size.y), color, Math::Vector2(uv.x,            uv.y + uvSize.y)},
    {Math::Vector2(topleft.x + size.x, topleft.y + size.y), color, Math::Vector2(uv.x + uvSize.x, uv.y + uvSize.y)},
    {Math::Vector2(topleft.x,          topleft.y         ), color, Math::Vector2(uv.x,            uv.y)},
    {Math::Vector2(topleft.x + size.x, topleft.y         ), color, Math::Vector2(uv.x + uvSize.x, uv.y)},
  }});
}

void Renderer2D::fillRect(const std::array<Vertex2D, 4>& v){
  createBufferSpace(4, 6);
  auto idx = vertexIdx;
  addVertex(v[0]);
  addVertex(v[1]);
  addVertex(v[2]);
  addVertex(v[3]);
  addIndex(idx);
  addIndex(idx + 1);
  addIndex(idx + 2);
  addIndex(idx + 2);
  addIndex(idx + 1);
  addIndex(idx + 3);
}

bool Renderer2D::createBufferSpace(unsigned int vertices, unsigned int indices){
  if(vertices > VERTEX_BUFFER_SIZE || indices > INDEX_BUFFER_SIZE) return false;
  auto verticesAvailable = VERTEX_BUFFER_SIZE - vertexIdx;
  auto indicesAvailable = INDEX_BUFFER_SIZE - indexIdx;
  if(vertices > verticesAvailable || indices > indicesAvailable){
    flush();
  }
  return true;
}

void Renderer2D::addVertex(const Vertex2D& v){
  vertices[vertexIdx].position = Math::Vector3(v.position.x, v.position.y, 0);
  vertices[vertexIdx].uv = v.uv;
  vertices[vertexIdx].color = v.color;
  vertexIdx ++;
}

void Renderer2D::addIndex(uint32_t idx){
  indices[indexIdx ++] = idx;
}

}