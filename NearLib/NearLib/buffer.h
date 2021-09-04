#pragma once

#include <d3d11.h>

#include "vertex.h"

namespace Near{

class IndexBuffer;

class VertexBuffer{
public:
  ~VertexBuffer();
  // 頂点バッファを作ります。
  // @param dynamic `set()`であとから中身を変えられるかどうか
  // @param size 頂点データのバイト数
  // @param data 頂点データ。`dynamic`が`false`の場合、`nullptr`にすると例外が投げられます。
  void init(bool dynamic, unsigned int size, const void* data);
  // 頂点バッファを解放します。
  void uninit();
  // `set()`でバッファの中身を変えられるかどうかを返します。
  bool isDynamic() const;
  // バッファの中身を変えます。`isDynamic()`が`false`のバッファで呼ぶと例外が投げられます。
  void set(const void* data);
  void set(const void* data, unsigned int size);
  // バッファの頂点を描画します。
  // @param stride 1頂点のバイト数 (`sizeof(Vertex3D)`など)
  // @param offset バッファの何バイト目から描画するか (3頂点目から描くには`sizeof(Vertex3D) * 2`)
  // @param vertexCount 頂点数
  // @param topology 描き方 (`D3D11_PRIMITIVE_TOPOLOGY_XXX`)
  void draw(unsigned int stride, unsigned int offset, unsigned int vertexCount, D3D11_PRIMITIVE_TOPOLOGY topology);
  void draw(unsigned int stride, unsigned int offset, unsigned int vertexCount, D3D11_PRIMITIVE_TOPOLOGY topology, const IndexBuffer& indices);
  ID3D11Buffer* getBuffer() const;
private:
  bool dynamic = false;
  unsigned int size = 0;
  ID3D11Buffer* buffer = nullptr;
};

class StandardVertexBuffer : public VertexBuffer{
public:
  void init(bool dynamic, unsigned int size, const void* data) = delete;
  // 頂点バッファを作ります。
  // @param dynamic `set()`であとから中身を変えられるかどうか
  // @param vertexCount 頂点数
  // @param vertices 頂点データ。`dynamic`が`false`の場合、`nullptr`にすると例外が投げられます。
  void init(bool dynamic, unsigned int vertexCount, const Vertex3D* vertices);
  void set(const void* data, unsigned int size) = delete;
  void set(const Vertex3D* vertices);
  void set(const Vertex3D* vertices, unsigned int vertexCount);
  void draw(unsigned int stride, unsigned int offset, unsigned int vertexCount, D3D11_PRIMITIVE_TOPOLOGY topology) = delete;
  void draw(unsigned int stride, unsigned int offset, unsigned int vertexCount, D3D11_PRIMITIVE_TOPOLOGY topology, const IndexBuffer& indices) = delete;
  // バッファの頂点を描画します。
  // @param vertexOffset 何頂点目から描画するか
  // @param vertexCount 頂点数
  // @param topology 描き方 (`D3D11_PRIMITIVE_TOPOLOGY_XXX`)
  void draw(unsigned int vertexOffset, unsigned int vertexCount, D3D11_PRIMITIVE_TOPOLOGY topology);
  void draw(unsigned int vertexOffset, unsigned int vertexCount, D3D11_PRIMITIVE_TOPOLOGY topology, const IndexBuffer& indices);
  // 全ての頂点を描画します。
  // @param topology 描き方 (`D3D11_PRIMITIVE_TOPOLOGY_XXX`)
  void draw(D3D11_PRIMITIVE_TOPOLOGY topology);
  void draw(D3D11_PRIMITIVE_TOPOLOGY topology, const IndexBuffer& indices);
private:
  unsigned int vertexCount = 0;
};

class IndexBuffer{
public:
  ~IndexBuffer();
  // インデックスバッファを作ります。
  // @param dynamic `set()`であとから中身を変えられるかどうか
  // @param indexCount インデックス数
  // @param indices インデックスデータ。`dynamic`が`false`の場合、`nullptr`にすると例外が投げられます。
  void init(bool dynamic, unsigned int indexCount, uint32_t* indices);
  // インデックスバッファを解放します。
  void uninit();
  unsigned int getIndexCount() const;
  // `set()`でバッファの中身を変えられるかどうかを返します。
  bool isDynamic() const;
  // バッファの中身を変えます。`isDynamic()`が`false`のバッファで呼ぶと例外が投げられます。
  void set(uint32_t* indices);
  ID3D11Buffer* getBuffer() const;
private:
  bool dynamic = false;
  unsigned int size = 0;
  unsigned int indexCount = 0;
  ID3D11Buffer* buffer = nullptr;
};

class ConstantBuffer{
public:
  ~ConstantBuffer();
  // 定数バッファを作ります。
  // @param dynamic `set()`であとから中身を変えられるかどうか
  // @param size データのバイト数
  // @param indices バッファデータ。`dynamic`が`false`の場合、`nullptr`にすると例外が投げられます。
  void init(bool dynamic, unsigned int size, void* data);
  void uninit();
  // `set()`でバッファの中身を変えられるかどうかを返します。
  bool isDynamic() const;
  // バッファの中身を変えます。`isDynamic()`が`false`のバッファで呼ぶと例外が投げられます。
  void set(const void* data);
  ID3D11Buffer* getBuffer() const;
private:
  bool dynamic = false;
  unsigned int size = 0;
  ID3D11Buffer* buffer = nullptr;
};

}
