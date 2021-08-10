#include "pch.h"
#include "renderer.h"
#include "utils.h"

#include <fstream>
#include <D3Dcompiler.h>

namespace Near{
Renderer::Renderer(){
}

Renderer::~Renderer(){
  uninit();
}

void Renderer::init(HWND window, int width, int height){
  this->window = window;
  this->width = width;
  this->height = height;

  HRESULT res;

  // デバイスとバッファスワップの設定

  DXGI_SWAP_CHAIN_DESC swapChainDesc{};
  swapChainDesc.BufferCount = 1;
  swapChainDesc.BufferDesc.Width = width;
  swapChainDesc.BufferDesc.Height = height;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = window;
  swapChainDesc.SampleDesc = createSampleDesc();
  swapChainDesc.Windowed = true;

  UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
  UINT createFactoryFlags = 0;
  #ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
  #endif

  IDXGIAdapter4* adapterToUse = nullptr;
  IDXGIFactory7* dxgiFactory = nullptr;
  try{
    if(SUCCEEDED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)))){
      IDXGIAdapter4* adapter;
      for(UINT i = 0;dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND;i ++){
        printf_s(u8"Adapter #%d: ", i);
        DXGI_ADAPTER_DESC3 desc;
        if(FAILED(adapter->GetDesc3(&desc))){
          printf_s(u8"*Error*");
          continue;
        }
        printf_s(u8"%s | %s | Video Memory: %zu\\n", narrow(desc.Description).c_str(), desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE ? u8"Software" : u8"Hardware", desc.DedicatedVideoMemory);
        if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE){
          continue;
        }
        adapterToUse = adapter;
        break;
      }
      dxgiFactory->Release();
    }
  }catch(...){} // VSのグラフィックスデバッグするとCreateDXGIFactory2で例外出す(´・ω・`)

  res = D3D11CreateDeviceAndSwapChain(
    adapterToUse,
    adapterToUse ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
    NULL,
    createDeviceFlags,
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &swapChainDesc,
    &swapChain,
    &device,
    &featureLevel,
    &deviceContext
  );
  if(FAILED(res)) throwResult("CreateDeviceAndSwapChain failed", res);

  ID3D11Texture2D* renderTarget = nullptr;
  res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&renderTarget));
  if(FAILED(res)) throwResult("GetBuffer failed", res);
  res = device->CreateRenderTargetView(renderTarget, nullptr, &renderTargetView);
  renderTarget->Release();
  if(FAILED(res)) throwResult("CreateRenderTargetView failed", res);

  ID3D11Texture2D* depthStencil = nullptr;
  D3D11_TEXTURE2D_DESC textureDesc{};
  textureDesc.Width = swapChainDesc.BufferDesc.Width;
  textureDesc.Height = swapChainDesc.BufferDesc.Height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  textureDesc.SampleDesc = createSampleDesc();
  textureDesc.Usage = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  textureDesc.CPUAccessFlags = 0;
  textureDesc.MiscFlags = 0;
  res = device->CreateTexture2D(&textureDesc, NULL, &depthStencil);
  if(FAILED(res)) throwResult("CreateTexture2D failed", res);

  D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
  depthStencilViewDesc.Format = textureDesc.Format;
  depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
  depthStencilViewDesc.Flags = 0;
  res = device->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &depthStencilView);
  depthStencil->Release();
  if(FAILED(res)) throwResult("CreateDepthStencilView failed", res);

  deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

  D3D11_VIEWPORT viewport{};
  viewport.Width = static_cast<FLOAT>(width);
  viewport.Height = static_cast<FLOAT>(height);
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  viewport.TopLeftX = 0;
  viewport.TopLeftY = 0;
  deviceContext->RSSetViewports(1, &viewport);

  D3D11_RASTERIZER_DESC rasterizerDesc{};
  rasterizerDesc.FillMode = D3D11_FILL_SOLID;
  rasterizerDesc.FrontCounterClockwise = true;
  rasterizerDesc.CullMode = D3D11_CULL_BACK;
  rasterizerDesc.DepthClipEnable = true;
  rasterizerDesc.MultisampleEnable = true;

  res = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
  if(FAILED(res)) throwResult("CreateRasterizerState (with culling) failed", res);
  deviceContext->RSSetState(rasterizerState);

  rasterizerDesc.CullMode = D3D11_CULL_NONE;
  res = device->CreateRasterizerState(&rasterizerDesc, &rasterizerStateNoCulling);
  if(FAILED(res)) throwResult("CreateRasterizerState (no culling) failed", res);
  
  D3D11_BLEND_DESC blendDesc{};
  blendDesc.AlphaToCoverageEnable = false;
  blendDesc.IndependentBlendEnable = false;
  blendDesc.RenderTarget[0].BlendEnable = true;
  blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  res = device->CreateBlendState(&blendDesc, &blendStateAlphaBlend);
  if(FAILED(res)) throwResult("CreateBlendState (alpha blend) failed", res);

  blendDesc.RenderTarget[0].BlendEnable = false;
  res = device->CreateBlendState(&blendDesc, &blendStateNoBlend);
  if(FAILED(res)) throwResult("CreateBlendState (no blend) failed", res);

  setBlendMode(BlendMode::ALPHA);

  // デプスステンシルステート設定
  D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
  depthStencilDesc.DepthEnable = true;
  depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
  depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
  depthStencilDesc.StencilEnable = false;
  res = device->CreateDepthStencilState(&depthStencilDesc, &depthState);
  if(FAILED(res)) throwResult("CreateDepthStencilState failed", res);
  deviceContext->OMSetDepthStencilState(depthState, 0);

  D3D11_SAMPLER_DESC samplerDesc{};
  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  device->CreateSamplerState(&samplerDesc, &samplerStateWrap);
  if(FAILED(res)) throwResult("CreateSamplerState failed", res);

  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  device->CreateSamplerState(&samplerDesc, &samplerStateClamp);
  if(FAILED(res)) throwResult("CreateSamplerState failed", res);

  setTextureAddressing(TextureAddressing::WRAP);

  // 座標変換をシェーダーに送る枠を作る

  D3D11_BUFFER_DESC bufferDesc{};
  bufferDesc.ByteWidth = sizeof(DirectX::SimpleMath::Matrix);
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;
  bufferDesc.StructureByteStride = sizeof(float);

  res = device->CreateBuffer(&bufferDesc, NULL, &worldBuffer);
  if(FAILED(res)) throwResult("CreateBuffer (world transform) failed", res);
  deviceContext->VSSetConstantBuffers(0, 1, &worldBuffer);

  res = device->CreateBuffer(&bufferDesc, NULL, &viewBuffer);
  if(FAILED(res)) throwResult("CreateBuffer (view transform) failed", res);
  deviceContext->VSSetConstantBuffers(1, 1, &viewBuffer);

  res = device->CreateBuffer(&bufferDesc, NULL, &projectionBuffer);
  if(FAILED(res)) throwResult("CreateBuffer (projection transform) failed", res);
  deviceContext->VSSetConstantBuffers(2, 1, &projectionBuffer);

  // Direct2D + DirectWrite

  res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
  if(FAILED(res)) throwResult("D2D1CreateFactory failed", res);

  IDXGIDevice1* dxgiDevice = nullptr;
  res = device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
  if(FAILED(res)) throwResult("QueryInterface for DXGIDevice failed", res);
  res = d2dFactory->CreateDevice(dxgiDevice, &d2dDevice);
  if(FAILED(res)) throwResult("Direct2D CreateDevice failed", res);
  res = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext);
  if(FAILED(res)) throwResult("Direct2D CreateDeviceContext failed", res);

  IDXGISurface1* backBuffer;
  swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

  FLOAT dpiX;
  FLOAT dpiY;
  d2dFactory->GetDesktopDpi(&dpiX, &dpiY);
  D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
    D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
    D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
    dpiX, dpiY
  );
  d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer, &bitmapProps, &d2dBitmap);

  d2dDeviceContext->CreateSolidColorBrush({1, 1, 1, 1}, &d2dBrush);

  DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2), reinterpret_cast<IUnknown**>(&dwFactory));
  dwFactory->CreateTextFormat(L"Inter", nullptr, DWRITE_FONT_WEIGHT_SEMI_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"ja-JP", reinterpret_cast<IDWriteTextFormat**>(&dwFormat));

  dwFactory->CreateTypography(&dwTypography);
  dwTypography->AddFontFeature({static_cast<DWRITE_FONT_FEATURE_TAG>(DWRITE_MAKE_OPENTYPE_TAG('c', 'v', '0', '3')), 1});
  dwTypography->AddFontFeature({static_cast<DWRITE_FONT_FEATURE_TAG>(DWRITE_MAKE_OPENTYPE_TAG('c', 'v', '0', '4')), 1});
  dwTypography->AddFontFeature({DWRITE_FONT_FEATURE_TAG_TABULAR_FIGURES, 1});

  IDWriteFontFallbackBuilder* fallbackBuilder;
  IDWriteFontFallback* fallback;
  DWRITE_UNICODE_RANGE range = {0, 0xffffffff};
  auto family = L"Noto Sans CJK JP";
  dwFactory->CreateFontFallbackBuilder(&fallbackBuilder);
  fallbackBuilder->AddMapping(&range, 1, &family, 1, nullptr, L"ja-jp");
  fallbackBuilder->CreateFontFallback(&fallback);
  dwFormat->SetFontFallback(fallback);
  
  fallback->Release();
  fallbackBuilder->Release();
}

void Renderer::uninit(){
  safeRelease(dwTypography);
  safeRelease(dwFormat);
  safeRelease(dwFactory);
  safeRelease(d2dBrush);
  safeRelease(d2dBitmap);
  safeRelease(d2dDeviceContext);
  safeRelease(d2dDevice);
  safeRelease(d2dFactory);
  safeRelease(projectionBuffer);
  safeRelease(viewBuffer);
  safeRelease(worldBuffer);
  safeRelease(samplerStateClamp);
  safeRelease(samplerStateWrap);
  safeRelease(blendStateNoBlend);
  safeRelease(blendStateAlphaBlend);
  safeRelease(rasterizerStateNoCulling);
  safeRelease(rasterizerState);
  safeRelease(depthState);
  safeRelease(depthStencilView);
  safeRelease(renderTargetView);
  safeRelease(swapChain);
  safeRelease(deviceContext);
  safeRelease(device);
}

void Renderer::clear(DirectX::SimpleMath::Color color){
  deviceContext->ClearRenderTargetView(renderTargetView, color);
  deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
  d2dDeviceContext->BeginDraw();
  d2dDeviceContext->SetTarget(d2dBitmap);
}

void Renderer::present(){
  d2dDeviceContext->EndDraw();
  swapChain->Present(1, 0);
}

void Renderer::drawText(std::string_view str, Math::Vector2 topleft){
  // std::wstring text = L"エラー(アクティブ) E0065 ';' が必要です NearLib renderer.cpp 219";
  std::wstring text = widen(str);
  IDWriteTextLayout* layout;
  dwFactory->CreateTextLayout(text.c_str(), text.size(), dwFormat, std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), &layout);
  layout->SetTypography(dwTypography, {0, 0xffffffff});
  d2dDeviceContext->DrawTextLayout({topleft.x, topleft.y}, layout, d2dBrush);
  layout->Release();
}

void Renderer::setCulling(bool cull){
  deviceContext->RSSetState(cull ? rasterizerState : rasterizerStateNoCulling);
}

BlendMode Renderer::getBlendMode(){
  return blend;
}

void Renderer::setBlendMode(BlendMode blend){
  this->blend = blend;
  float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  switch(blend){
    case BlendMode::ALPHA: deviceContext->OMSetBlendState(blendStateAlphaBlend, blendFactor, 0xffffffff); break;
    default:               deviceContext->OMSetBlendState(blendStateNoBlend, blendFactor, 0xffffffff); break;
  }
}

TextureAddressing Renderer::getTextureAddressing(){
  return addressing;
}

void Renderer::setTextureAddressing(TextureAddressing addressing){
  this->addressing = addressing;
  switch(addressing){
    case TextureAddressing::WRAP:  deviceContext->PSSetSamplers(0, 1, &samplerStateWrap); break;
    case TextureAddressing::CLAMP: deviceContext->PSSetSamplers(0, 1, &samplerStateClamp); break;
    default: break;
  }
}

DirectX::SimpleMath::Matrix Renderer::getWorldTransform(){
  return worldTransform;
}

DirectX::SimpleMath::Matrix Renderer::getViewTransform(){
  return wiewTransform;
}

DirectX::SimpleMath::Matrix Renderer::getProjectionTransform(){
  return wrojectionTransform;
}

void Renderer::setWorldTransform(DirectX::SimpleMath::Matrix transform){
  worldTransform = transform;
  DirectX::SimpleMath::Matrix t;
  transform.Transpose(t);
  deviceContext->UpdateSubresource(worldBuffer, 0, nullptr, &t, 0, 0);
}

void Renderer::setViewTransform(DirectX::SimpleMath::Matrix transform){
  wiewTransform = transform;
  DirectX::SimpleMath::Matrix t;
  transform.Transpose(t);
  deviceContext->UpdateSubresource(viewBuffer, 0, nullptr, &t, 0, 0);
}

void Renderer::setProjectionTransform(DirectX::SimpleMath::Matrix transform){
  wrojectionTransform = transform;
  DirectX::SimpleMath::Matrix t;
  transform.Transpose(t);
  deviceContext->UpdateSubresource(projectionBuffer, 0, nullptr, &t, 0, 0);
}

void Renderer::applyWorldTransform(DirectX::SimpleMath::Matrix transform){
  setWorldTransform(worldTransform * transform);
}

void Renderer::pushWorldTransform(){
  worldTransforms.push_back(worldTransform);
}

void Renderer::popWorldTransform(){
  if(worldTransforms.empty()) return;
  setWorldTransform(*worldTransforms.rbegin());
  worldTransforms.pop_back();
}

void Renderer::compileVertexShader(ID3D11VertexShader** shader, ID3D11InputLayout** layout, const char* path){
  std::string fileContent;
  {
    std::ifstream stream(path);
    fileContent = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    stream.close();
  }

  ID3DBlob* code;
  ID3DBlob* errorMessages;
  HRESULT res = D3DCompile(fileContent.c_str(), fileContent.size(), path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", 0, 0, &code, &errorMessages);
  if(FAILED(res)){
    // char* a = (char*)errorMessages->GetBufferPointer();
    safeRelease(code);
    safeRelease(errorMessages);
    throwResult("Vertex shader compile failed", res);
  }
  
  res = device->CreateVertexShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, shader);
  if(FAILED(res)){
    safeRelease(code);
    safeRelease(errorMessages);
    throwResult("CreateVertexShader failed", res);
  }

  D3D11_INPUT_ELEMENT_DESC descs[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,      D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 4 *  3, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 *  6, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0}
  };
  UINT numElements = ARRAYSIZE(descs);

  res = device->CreateInputLayout(
    descs,
    numElements,
    code->GetBufferPointer(),
    code->GetBufferSize(),
    layout
  );
  safeRelease(code);
  safeRelease(errorMessages);

  if(FAILED(res)) throwResult("CreateInputLayout failed", res);
}

void Renderer::compilePixelShader(ID3D11PixelShader** shader, const char* path){
  std::string fileContent;
  {
    std::ifstream stream(path);
    fileContent = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    stream.close();
  }

  ID3DBlob* code;
  ID3DBlob* errorMessages;
  HRESULT res = D3DCompile(fileContent.c_str(), fileContent.size(), path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", 0, 0, &code, &errorMessages);
  if(FAILED(res)){
    char* a = (char*)errorMessages->GetBufferPointer();
    safeRelease(code);
    safeRelease(errorMessages);
    throwResult("Vertex shader compile failed", res);
  }
  
  res = device->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, shader);
  safeRelease(code);
  safeRelease(errorMessages);
  if(FAILED(res)) throwResult("CreatePixelShader failed", res);
}

void Renderer::createVertexShader(ID3D11VertexShader** shader, ID3D11InputLayout** layout, const char* path){
  char* buffer = nullptr;
  std::streampos fileSize = 0;

  {
    std::ifstream stream(path, std::ios_base::binary | std::ios_base::ate);
    if(!stream) throw std::invalid_argument(std::string("Can't open vertex shader: ").append(path));

    fileSize = stream.tellg();
    stream.seekg(0);

    buffer = new char[fileSize];
    stream.read(buffer, fileSize);
  }

  HRESULT res = device->CreateVertexShader(buffer, fileSize, NULL, shader);
  if(FAILED(res)){
    delete[] buffer;
    throwResult("CreateVertexShader failed", res);
  }

  D3D11_INPUT_ELEMENT_DESC descs[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,      D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 4 *  3, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 *  6, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0}
  };
  UINT numElements = ARRAYSIZE(descs);

  res = device->CreateInputLayout(
    descs,
    numElements,
    buffer,
    fileSize,
    layout
  );
  delete[] buffer;

  if(FAILED(res)) throwResult("CreateInputLayout failed", res);
}

void Renderer::createPixelShader(ID3D11PixelShader** shader, const char* path){
  char* buffer = nullptr;
  std::streampos fileSize = 0;

  {
    std::ifstream stream(path, std::ios_base::binary | std::ios_base::ate);
    if(!stream) throw std::invalid_argument(std::string("Can't open pixel shader: ").append(path));

    fileSize = stream.tellg();
    stream.seekg(0);

    buffer = new char[fileSize];
    stream.read(buffer, fileSize);
  }

  HRESULT res = device->CreatePixelShader(buffer, fileSize, NULL, shader);
  delete[] buffer;
  if(FAILED(res)) throwResult("CreateInputLayout failed", res);
}

void Renderer::setVertexShader(VertexShader* shader){
  deviceContext->IASetInputLayout(shader->layout);
  deviceContext->VSSetShader(shader->shader, nullptr, 0);
}

void Renderer::setPixelShader(PixelShader* shader){
  deviceContext->PSSetShader(shader->shader, nullptr, 0);
}

void Renderer::setTexture(Texture* texture){
  if(texture){
    deviceContext->PSSetShaderResources(0, 1, &texture->texture);
  }else{
    // useSampler = false 的なのシェーダーに送る？
  }
}

int Renderer::getWidth(){
  return width;
}

int Renderer::getHeight(){
  return height;
}

HWND Renderer::getWindow(){
  return window;
}

ID3D11Device* Renderer::getDevice(){
  return device;
}

ID3D11DeviceContext* Renderer::getDeviceContext(){
  return deviceContext;
}

DXGI_SAMPLE_DESC Renderer::createSampleDesc(){
  return {4, 0};
}

void Renderer::resetRenderTarget(){
  deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

}
