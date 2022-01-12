#include "portal-scene.h"

#include <NearLib/vertex.h>
#include <NearLib/utils.h>

#include "main.h"

PortalScene::PortalScene(){
}

void PortalScene::init(){
  Near::Scene::init();
  
  auto* renderer = Near::renderer();
  CD3D11_DEPTH_STENCIL_DESC desc(D3D11_DEFAULT);
  desc.DepthEnable = true;
  desc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
  desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
  desc.StencilEnable = true;
  desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
  desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
  desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
  desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
  desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

  HRESULT res;
  res = renderer->getDevice()->CreateDepthStencilState(&desc, &stencilState);
  if(FAILED(res)) Near::throwResult("CreateDepthStencilState failed", res);

  desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
  res = renderer->getDevice()->CreateDepthStencilState(&desc, &stencilStateIncr);
  if(FAILED(res)) Near::throwResult("CreateDepthStencilState failed", res);

  desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
  desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
  res = renderer->getDevice()->CreateDepthStencilState(&desc, &stencilStateDecr);
  if(FAILED(res)) Near::throwResult("CreateDepthStencilState failed", res);

  // desc.DepthEnable = false;
  desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  res = renderer->getDevice()->CreateDepthStencilState(&desc, &stencilStateClearDepth);
  if(FAILED(res)) Near::throwResult("CreateDepthStencilState failed", res);

  Near::Vertex3D vertices[] = {
    {Near::Math::Vector3(-1, -1, 1), Near::Math::Vector3(0, 0, 1), NearGame::BACKGROUND_COLOR,    Near::Math::Vector2(0, 1)},
    {Near::Math::Vector3( 1, -1, 1), Near::Math::Vector3(0, 0, 1), NearGame::BACKGROUND_COLOR,    Near::Math::Vector2(1, 1)},
    {Near::Math::Vector3(-1,  1, 1), Near::Math::Vector3(0, 0, 1), NearGame::BACKGROUND_COLOR,    Near::Math::Vector2(0, 0)},
    {Near::Math::Vector3( 1,  1, 1), Near::Math::Vector3(0, 0, 1), NearGame::BACKGROUND_COLOR,    Near::Math::Vector2(1, 0)},
  };
  fullscreenQuads.init(false, 4, vertices);

  // Render Texture (いろ)
  CD3D11_TEXTURE2D_DESC texDesc(DXGI_FORMAT_R8G8B8A8_UNORM, renderer->getWidth(), renderer->getHeight(), 1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
  ID3D11Texture2D* texture;
  res = renderer->getDevice()->CreateTexture2D(&texDesc, nullptr, &texture);
  if(FAILED(res)) Near::throwResult("CreateTexture2D failed", res);
  res = renderer->getDevice()->CreateShaderResourceView(texture, nullptr, &renderTexture);
  if(FAILED(res)) Near::throwResult("CreateShaderResourceView failed", res);

  res = renderer->getDevice()->CreateRenderTargetView(texture, nullptr, &renderView);
  if(FAILED(res)) Near::throwResult("CreateRenderTargetView failed", res);
  texture->Release();

  // Render Texture (ほうせん)
  texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
  res = renderer->getDevice()->CreateTexture2D(&texDesc, nullptr, &texture);
  if(FAILED(res)) Near::throwResult("CreateTexture2D failed", res);
  res = renderer->getDevice()->CreateShaderResourceView(texture, nullptr, &renderTextureNormal);
  if(FAILED(res)) Near::throwResult("CreateShaderResourceView failed", res);

  res = renderer->getDevice()->CreateRenderTargetView(texture, nullptr, &renderViewNormal);
  if(FAILED(res)) Near::throwResult("CreateRenderTargetView failed", res);
  texture->Release();

  // Render Texture (しんど)
  texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
  texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  res = renderer->getDevice()->CreateTexture2D(&texDesc, nullptr, &texture);
  if(FAILED(res)) Near::throwResult("CreateTexture2D failed", res);

  D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc{};
  depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  res = renderer->getDevice()->CreateDepthStencilView(texture, &depthDesc, &renderViewDepth);
  if(FAILED(res)) Near::throwResult("CreateDepthStencilView failed", res);

  CD3D11_SHADER_RESOURCE_VIEW_DESC rtDesc(texture, D3D_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
  res = renderer->getDevice()->CreateShaderResourceView(texture, &rtDesc, &renderTextureDepth);
  if(FAILED(res)) Near::throwResult("CreateShaderResourceView failed", res);

  texture->Release();

  vertexShader = Near::Assets::vertexShaders()->getOrLoad("assets/nearlib/shaders/vs.hlsl");
  pixelShader = Near::Assets::pixelShaders()->getOrLoad("assets/shaders/ps-level-postprocess.hlsl");
  pixelShaderClearNormal = Near::Assets::pixelShaders()->getOrLoad("assets/shaders/ps-clearnormal.hlsl");
}

void PortalScene::uninit(){
  fullscreenQuads.uninit();
  vertexShader.reset();
  pixelShader.reset();
  Near::safeRelease(renderView);
  Near::safeRelease(renderViewNormal);
  Near::safeRelease(renderViewDepth);
  Near::safeRelease(renderTexture);
  Near::safeRelease(renderTextureNormal);
  Near::safeRelease(renderTextureDepth);
  Near::safeRelease(stencilStateClearDepth);
  Near::safeRelease(stencilStateDecr);
  Near::safeRelease(stencilStateIncr);
  Near::safeRelease(stencilState);
  Near::Scene::uninit();
}

void PortalScene::draw(){
  auto* renderer = Near::renderer();

  layers[LAYER_MANAGERS].draw();

  portals.clear();
  findObjectsOfExactType<Portal>(portals);
  camera = findObjectOfType<Near::Camera>();
  cameraTransform = camera->transform;

  ID3D11DepthStencilState* prevState;
  UINT prevRef;
  renderer->getDeviceContext()->OMGetDepthStencilState(&prevState, &prevRef);

  ID3D11RenderTargetView* views[] = {
    renderView,
    renderViewNormal,
  };
  renderer->getDeviceContext()->OMSetRenderTargets(2, views, renderViewDepth);
  renderer->getDeviceContext()->ClearRenderTargetView(renderView, NearGame::BACKGROUND_COLOR);
  renderer->getDeviceContext()->ClearRenderTargetView(renderViewNormal, Near::Math::Color(0, 0, 0, 0));
  renderer->getDeviceContext()->ClearDepthStencilView(renderViewDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

  drawRecurse(0);
  renderer->resetRenderTarget();
  renderer->getDeviceContext()->OMSetDepthStencilState(prevState, prevRef);

  renderer->setVertexShader(vertexShader.get());
  renderer->setPixelShader(pixelShader.get());
  ID3D11ShaderResourceView* textures[] = {renderTexture, renderTextureNormal, renderTextureDepth};
  renderer->getDeviceContext()->PSSetShaderResources(0, 3, textures);
  Near::TextureAddressing addr = renderer->getTextureAddressing();
  renderer->setTextureAddressing(Near::TextureAddressing::CLAMP);
  drawFullscreenQuad(false);
  renderer->setTextureAddressing(addr);
  textures[0] = nullptr;
  textures[1] = nullptr;
  textures[2] = nullptr;
  renderer->getDeviceContext()->PSSetShaderResources(0, 3, textures);

  layers[LAYER_OVERLAY].draw();
}

int PortalScene::getPortalLevel() const{
  return portalLevel;
}

void PortalScene::drawRecurse(int level){
  Near::Math::Vector3 tmpVector;
  Near::Math::Quaternion tmpQuaternion;
  auto* renderer = Near::renderer();
  // 1. 不透明オブジェクトを表示
  portalLevel = level;
  renderer->setBlendMode(Near::BlendMode::NONE);
  layers[LAYER_OBJECTS].draw();
  if(level < 2){
    int i = 0;
    for(auto portal : portals){
      std::shared_ptr<Portal> otherPortal = portal->otherPortal.lock();
      if(!otherPortal) continue;
      if(!isPortalVisible(portal)) continue;

      // 2. ポータルのステンシルを書き込む
      renderer->getDeviceContext()->OMSetDepthStencilState(stencilStateIncr, static_cast<UINT>(level));
      portal->draw();
      // 3. ポータル面の深度を消す
      renderer->getDeviceContext()->OMSetDepthStencilState(stencilStateClearDepth, static_cast<UINT>(level + 1));
      drawFullscreenQuad(true);
      // 4. ポータルの先を表示
      auto cameraTransform = camera->transform;
      
      // カメラ - 入口ポータル + 180度 + 出口ポータル
      // 1. カメラのワールド座標
      // 2. 入口ポータルとの相対にする
      // 3. 後ろを向く
      // 4. 出口ポータルを足すといい感じのワールド座標に戻る
      Near::Math::Matrix m = camera->transform.createTransform() * portal->transform.createTransform().Invert() * Near::Math::Matrix::CreateFromYawPitchRoll(DirectX::XM_PI, 0, 0) * otherPortal->transform.createTransform();
      Near::Math::Vector3 dummy;
      m.Decompose(dummy, camera->transform.rotation, camera->transform.position);

      camera->draw();

      Near::Math::Plane portalPlane(otherPortal->transform.position, otherPortal->transform.getForward());
      renderer->setProjectionTransform(Near::createObliqueMatrix(renderer->getViewTransform(), renderer->getProjectionTransform(), portalPlane));

      renderer->getDeviceContext()->OMSetDepthStencilState(stencilState, static_cast<UINT>(level + 1));
      drawRecurse(level + 1);
      renderer->getDeviceContext()->OMSetDepthStencilState(stencilStateDecr, static_cast<UINT>(level + 1));
      camera->transform = cameraTransform;
      camera->draw();
      portal->draw();
      // drawFullscreenQuad();
      // camera->draw();
    }
  }
  // 5. 透明オブジェクトを表示
  portalLevel = level;
  renderer->setBlendMode(Near::BlendMode::ALPHA);
  layers[LAYER_TRANSPARENT_OBJECTS].draw();
}

void PortalScene::drawFullscreenQuad(bool isBackground){
  auto* renderer = Near::renderer();
  renderer->setVertexShader(vertexShader.get());
  renderer->setPixelShader((isBackground ? pixelShaderClearNormal : pixelShader).get());
  renderer->pushWorldTransform();
  renderer->setWorldTransform(Near::Math::Matrix::Identity);
  renderer->setViewTransform(Near::Math::Matrix::Identity);
  renderer->setProjectionTransform(Near::Math::Matrix::Identity);
  fullscreenQuads.draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  renderer->popWorldTransform();
}

bool PortalScene::isPortalVisible(std::shared_ptr<Portal> portal){
  auto& extents = portal->getExtents();
  Near::Math::Vector3 right = portal->transform.getRight() * extents.x;
  Near::Math::Vector3 up    = portal->transform.getUp() * extents.y;
  Near::Math::Vector3 p0 = portal->transform.position - right + up;
  Near::Math::Vector3 p1 = portal->transform.position + right + up;
  Near::Math::Vector3 p2 = portal->transform.position - right - up;
  Near::Math::Vector3 p3 = portal->transform.position + right - up;
  Near::Math::Matrix a;
  DirectX::BoundingFrustum frustum;
  Near::createBoundingFrustum(frustum, camera->createProjectionTransform(), true);
  frustum.Transform(frustum, camera->createViewTransform().Invert());

  auto res = frustum.Contains(p0, p1, p2);
  if(res == DirectX::ContainmentType::INTERSECTS || res == DirectX::ContainmentType::CONTAINS) return true;
  res = frustum.Contains(p2, p1, p3);
  if(res == DirectX::ContainmentType::INTERSECTS || res == DirectX::ContainmentType::CONTAINS) return true;

  return false;
}
