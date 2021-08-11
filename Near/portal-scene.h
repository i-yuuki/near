#pragma once

#include <NearLib/scene.h>
#include <NearLib/camera.h>
#include <NearLib/buffer.h>
#include "portal.h"

class PortalScene : public Near::Scene{
public:
  PortalScene();
  virtual void init() override;
  virtual void uninit() override;
  virtual void draw() override;
  int getPortalLevel() const;
private:
  std::vector<std::shared_ptr<Portal>> portals;
  std::shared_ptr<Near::Camera> camera;
  Near::Transform cameraTransform;
  ID3D11DepthStencilState* stencilState;
  ID3D11DepthStencilState* stencilStateIncr;
  ID3D11DepthStencilState* stencilStateDecr;
  ID3D11DepthStencilState* stencilStateClearDepth;
  ID3D11ShaderResourceView* renderTexture;
  ID3D11ShaderResourceView* renderTextureDepth;
  ID3D11ShaderResourceView* renderTextureNormal;
  ID3D11RenderTargetView* renderView;
  ID3D11RenderTargetView* renderViewNormal;
  ID3D11DepthStencilView* renderViewDepth;
  std::shared_ptr<Near::VertexShader> vertexShader;
  std::shared_ptr<Near::PixelShader> pixelShader;
  std::shared_ptr<Near::PixelShader> pixelShaderNoTex;
  Near::StandardVertexBuffer fullscreenQuads;
  int portalLevel;
  void drawRecurse(int level);
  void drawFullscreenQuad(bool isBackground);
};
