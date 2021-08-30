#include "pch.h"
#include "texture.h"

#include <wincodec.h>

#include "near.h"
#include "utils.h"
#include "DirectXTK/WICTextureLoader.h"

namespace Near{

void Texture::load(const std::string& path){
  HRESULT res = DirectX::CreateWICTextureFromFileEx(renderer()->getDevice(), widen(path).c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_IGNORE_SRGB, nullptr, &texture);
  if(FAILED(res)) throwResult(std::string("Failed to load texture from file ").append(path).c_str(), res);
  /* あら～DXTKべんり～
  HRESULT res;
  IWICImagingFactory* wicFactory = nullptr;
  IWICBitmapDecoder *decoder = nullptr;
  IWICBitmapFrameDecode* frame = nullptr;
  IWICBitmapSource* bitmap = nullptr;
  unsigned char* pixels = nullptr;
  UINT width = 0;
  UINT height = 0;
  try{
    res = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<LPVOID*>(&wicFactory));
    if(FAILED(res)) throwResult("Texture::load: CoCreateInstance failed", res);
    res = wicFactory->CreateDecoderFromFilename(
      widen(path).c_str(),
      nullptr,
      GENERIC_READ,
      WICDecodeMetadataCacheOnDemand,
      &decoder
    );
    if(FAILED(res)) throwResult("Texture::load: CreateDecoderFromFilename failed", res);
    res = decoder->GetFrame(0, &frame);
    if(FAILED(res)) throwResult("Texture::load: GetFrame failed", res);
    res = WICConvertBitmapSource(GUID_WICPixelFormat32bppRGBA, frame, &bitmap);
    if(FAILED(res)) throwResult("Texture::load: WICConvertBitmapSource failed", res);
    frame->GetSize(&width, &height);
    pixels = new unsigned char[width * height * 4];
    res = bitmap->CopyPixels(nullptr, width * 4, width * height * 4, pixels);
    if(FAILED(res)) throwResult("Texture::load: CopyPixels failed", res);

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = pixels;
    initData.SysMemPitch = static_cast<UINT>(width * 4);
    initData.SysMemSlicePitch = static_cast<UINT>(width * height * 4);

    res = renderer()->getDevice()->CreateTexture2D(&desc, &initData, &texture);
    if(FAILED(res)) throwResult("Texture::load: CreateTexture2D failed", res);
  }catch(...){
    safeRelease(wicFactory);
    safeRelease(decoder);
    safeRelease(frame);
    safeRelease(bitmap);
    safeDeleteArray(pixels);
    throw;
  }
  safeRelease(wicFactory);
  safeRelease(decoder);
  safeRelease(frame);
  safeRelease(bitmap);
  safeDeleteArray(pixels);
  */
}

Texture::~Texture(){
  safeRelease(texture);
}

}
