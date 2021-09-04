#include "pch.h"
#include "buffer.h"

#include "near.h"
#include "utils.h"

namespace Near{

VertexBuffer::~VertexBuffer(){
  uninit();
}

void VertexBuffer::init(bool dynamic, unsigned int size, const void* data){
  if(!dynamic && !data) throw std::exception("No data provided to non-dynamic VertexBuffer::init()");

  this->dynamic = dynamic;
  this->size = size;

  HRESULT res;

  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = size;
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  if(dynamic){
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  }

  D3D11_SUBRESOURCE_DATA subresource = {};
  subresource.pSysMem = data;

  res = renderer()->getDevice()->CreateBuffer(&bufferDesc, data ? &subresource : nullptr, &buffer);
  if(FAILED(res)) throwResult("VertexBuffer CreateBuffer failed", res);
}

void VertexBuffer::uninit(){
  safeRelease(buffer);
}

bool VertexBuffer::isDynamic() const{
  return dynamic;
}

void VertexBuffer::set(const void* data){
  set(data, size);
}

void VertexBuffer::set(const void* data, unsigned int size){
  if(!dynamic) throw std::exception("Non-dynamic VertexBuffer::set() was called");

  HRESULT res;

  D3D11_MAPPED_SUBRESOURCE resource;
  res = renderer()->getDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
  if(FAILED(res)) throwResult("VertexBuffer Map failed", res);
  std::memcpy(resource.pData, data, size);
  renderer()->getDeviceContext()->Unmap(buffer, 0);
}

void VertexBuffer::draw(unsigned int stride, unsigned int offset, unsigned int vertexCount, D3D11_PRIMITIVE_TOPOLOGY topology){
  auto* r = renderer();
  r->getDeviceContext()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
  r->getDeviceContext()->IASetPrimitiveTopology(topology);
  r->getDeviceContext()->Draw(vertexCount, 0);
}

void VertexBuffer::draw(unsigned int stride, unsigned int offset, unsigned int indexCount, D3D11_PRIMITIVE_TOPOLOGY topology, const IndexBuffer& indices){
  auto* r = renderer();
  r->getDeviceContext()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
  r->getDeviceContext()->IASetIndexBuffer(indices.getBuffer(), DXGI_FORMAT_R32_UINT, 0);
  r->getDeviceContext()->IASetPrimitiveTopology(topology);
  r->getDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

ID3D11Buffer* VertexBuffer::getBuffer() const{
  return buffer;
}

void StandardVertexBuffer::init(bool dynamic, unsigned int vertexCount, const Vertex3D* vertices){
  VertexBuffer::init(dynamic, sizeof(Vertex3D) * vertexCount, vertices);
  this->vertexCount = vertexCount;
}

void StandardVertexBuffer::set(const Vertex3D* vertices){
  VertexBuffer::set(vertices);
}

void StandardVertexBuffer::set(const Vertex3D* vertices, unsigned int vertexCount){
  VertexBuffer::set(vertices, sizeof(Vertex3D) * vertexCount);
}

void StandardVertexBuffer::draw(D3D11_PRIMITIVE_TOPOLOGY topology){
  draw(0, vertexCount, topology);
}

void StandardVertexBuffer::draw(unsigned int vertexOffset, unsigned int vertexCount, D3D11_PRIMITIVE_TOPOLOGY topology){
  VertexBuffer::draw(sizeof(Vertex3D), 0, vertexCount, topology);
}

void StandardVertexBuffer::draw(unsigned int vertexOffset, unsigned int indexCount, D3D11_PRIMITIVE_TOPOLOGY topology, const IndexBuffer& indices){
  VertexBuffer::draw(sizeof(Vertex3D), sizeof(Vertex3D) * vertexOffset, indexCount, topology, indices);
}

void StandardVertexBuffer::draw(D3D11_PRIMITIVE_TOPOLOGY topology, const IndexBuffer& indices){
  draw(0, indices.getIndexCount(), topology, indices);
}

IndexBuffer::~IndexBuffer(){
  uninit();
}

void IndexBuffer::init(bool dynamic, unsigned int indexCount, uint32_t* indices){
  if(!dynamic && !indices) throw std::exception("No data provided to non-dynamic VertexBuffer::init()");

  this->dynamic = dynamic;
  this->indexCount = indexCount;
  this->size = indexCount * 4;

  HRESULT res;

  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = size;
  bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  if(dynamic){
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  }

  D3D11_SUBRESOURCE_DATA subresource = {};
  subresource.pSysMem = indices;

  res = renderer()->getDevice()->CreateBuffer(&bufferDesc, indices ? &subresource : nullptr, &buffer);
  if(FAILED(res)) throwResult("IndexBuffer CreateBuffer failed", res);
}

void IndexBuffer::uninit(){
  safeRelease(buffer);
}

unsigned int IndexBuffer::getIndexCount() const{
  return indexCount;
}

bool IndexBuffer::isDynamic() const{
  return dynamic;
}

void IndexBuffer::set(uint32_t* indices){
  if(!dynamic) throw std::exception("Non-dynamic IndexBuffer::set() was called");

  HRESULT res;

  D3D11_MAPPED_SUBRESOURCE resource;
  res = renderer()->getDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
  if(FAILED(res)) throwResult("IndexBuffer Map failed", res);
  std::memcpy(resource.pData, indices, size);
  renderer()->getDeviceContext()->Unmap(buffer, 0);
}

ID3D11Buffer* IndexBuffer::getBuffer() const{
  return buffer;
}

ConstantBuffer::~ConstantBuffer(){
  uninit();
}

void ConstantBuffer::init(bool dynamic, unsigned int size, void* data){
  // if(size > D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT) throw std::exception(std::string("Constant buffer too large: ").append(std::to_string(size).append(" > ").append(std::to_string(D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT))));
  if(!dynamic && !data) throw std::exception("No data provided to non-dynamic ConstantBuffer::init()");

  // 定数バッファはサイズが16の倍数
  if(size % 16){
    size += 16 - (size % 16);
  }

  this->dynamic = dynamic;
  this->size = size;

  HRESULT res;

  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = size;
  bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  if(dynamic){
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  }

  D3D11_SUBRESOURCE_DATA subresource = {};
  subresource.pSysMem = data;

  res = renderer()->getDevice()->CreateBuffer(&bufferDesc, data ? &subresource : nullptr, &buffer);
  if(FAILED(res)) throwResult("ConstantBuffer CreateBuffer failed", res);
}

void ConstantBuffer::uninit(){
  safeRelease(buffer);
}

bool ConstantBuffer::isDynamic() const{
  return dynamic;
}

void ConstantBuffer::set(const void* data){
  if(!dynamic) throw std::exception("Non-dynamic ConstantBuffer::set() was called");

  HRESULT res;

  D3D11_MAPPED_SUBRESOURCE resource;
  res = renderer()->getDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
  if(FAILED(res)) throwResult("ConstantBuffer Map failed", res);
  std::memcpy(resource.pData, data, size);
  renderer()->getDeviceContext()->Unmap(buffer, 0);
}

ID3D11Buffer* ConstantBuffer::getBuffer() const{
  return buffer;
}

}
