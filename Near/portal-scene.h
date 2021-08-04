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
private:
  std::vector<std::shared_ptr<Portal>> portals;
  std::shared_ptr<Near::Camera> camera;
  Near::Transform cameraTransform;
  ID3D11DepthStencilState* stencilState;
  ID3D11DepthStencilState* stencilStateIncr;
  ID3D11DepthStencilState* stencilStateDecr;
  ID3D11DepthStencilState* stencilStateClearDepth;
  Near::VertexBuffer fullscreenQuad;
  void drawRecurse(int level);
  void drawFullscreenQuad();
};
