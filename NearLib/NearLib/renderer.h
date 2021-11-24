#pragma once

#include <d2d1_1.h>
#include <dwrite_2.h>

#include "DirectXTK/SimpleMath.h"
#include "shader.h"
#include "texture.h"

namespace Near{

enum class BlendMode{
  NONE,
  ALPHA,
};

enum class TextureAddressing{
  WRAP,
  CLAMP,
};

class Renderer{
public:
  Renderer();
  ~Renderer();
  void init(HWND window, int width, int height);
  void uninit();
  void clear(DirectX::SimpleMath::Color color);
  void present();
  void drawText(std::string_view str, DirectX::SimpleMath::Vector2 topleft);
  void setCulling(bool cull);
  BlendMode getBlendMode();
  void setBlendMode(BlendMode blend);
  TextureAddressing getTextureAddressing();
  void setTextureAddressing(TextureAddressing addressing);
  DirectX::SimpleMath::Matrix getWorldTransform();
  DirectX::SimpleMath::Matrix getViewTransform();
  DirectX::SimpleMath::Matrix getProjectionTransform();
  void setWorldTransform(DirectX::SimpleMath::Matrix transform);
  void setViewTransform(DirectX::SimpleMath::Matrix transform);
  void setProjectionTransform(DirectX::SimpleMath::Matrix transform);
  void applyWorldTransform(DirectX::SimpleMath::Matrix transform);
  void pushWorldTransform();
  void popWorldTransform();
  void compileVertexShader(ID3D11VertexShader** shader, ID3D11InputLayout** layout, const char* path);
  void compilePixelShader(ID3D11PixelShader** shader, const char* path);
  void createVertexShader(ID3D11VertexShader** shader, ID3D11InputLayout** layout, const char* path);
  void createPixelShader(ID3D11PixelShader** shader, const char* path);
  void setVertexShader(VertexShader* shader);
  void setPixelShader(PixelShader* shader);
  void setTexture(Texture* texture);
  int getWidth();
  int getHeight();
  HWND getWindow();
  ID3D11Device* getDevice();
  ID3D11DeviceContext* getDeviceContext();
  DXGI_SAMPLE_DESC createSampleDesc();
  void resetRenderTarget();
private:
  HWND window = NULL;
  int width = 0;
  int height = 0;
  D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
  ID3D11Device* device = nullptr;
  ID3D11DeviceContext* deviceContext = nullptr;
  IDXGISwapChain* swapChain = nullptr;
  ID3D11RenderTargetView* renderTargetView = nullptr;
  ID3D11DepthStencilView* depthStencilView = nullptr;
  ID3D11DepthStencilState* depthState = nullptr;
  ID3D11RasterizerState* rasterizerState = nullptr;
  ID3D11RasterizerState* rasterizerStateNoCulling = nullptr;
  BlendMode blend;
  TextureAddressing addressing;
  ID3D11BlendState* blendStateAlphaBlend = nullptr;
  ID3D11BlendState* blendStateNoBlend = nullptr;
  ID3D11SamplerState* samplerStateWrap = nullptr;
  ID3D11SamplerState* samplerStateClamp = nullptr;
  ID3D11Buffer* worldBuffer = nullptr;
  ID3D11Buffer* viewBuffer = nullptr;
  ID3D11Buffer* projectionBuffer = nullptr;
  DirectX::SimpleMath::Matrix worldTransform;
  DirectX::SimpleMath::Matrix wiewTransform;
  DirectX::SimpleMath::Matrix wrojectionTransform;
  std::vector<DirectX::SimpleMath::Matrix> worldTransforms;
};

}
