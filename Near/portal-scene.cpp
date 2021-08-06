#include "portal-scene.h"

#include <NearLib/vertex.h>
#include <NearLib/utils.h>

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
    {Near::Math::Vector3(-1, -1, 1), Near::Math::Vector3(0, 0, 1), Near::Math::Color(1, 1, 1, 1), Near::Math::Vector2(0, 0)},
    {Near::Math::Vector3( 1, -1, 1), Near::Math::Vector3(0, 0, 1), Near::Math::Color(1, 1, 1, 1), Near::Math::Vector2(1, 0)},
    {Near::Math::Vector3(-1,  1, 1), Near::Math::Vector3(0, 0, 1), Near::Math::Color(1, 1, 1, 1), Near::Math::Vector2(0, 1)},
    {Near::Math::Vector3( 1,  1, 1), Near::Math::Vector3(0, 0, 1), Near::Math::Color(1, 1, 1, 1), Near::Math::Vector2(1, 1)},
  };
  fullscreenQuad.init(false, sizeof(vertices), vertices);
}

void PortalScene::uninit(){
  fullscreenQuad.uninit();
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
  drawRecurse(0);
  renderer->getDeviceContext()->OMSetDepthStencilState(prevState, prevRef);

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
  if(level < 1){
    int i = 0;
    for(auto portal : portals){
      std::shared_ptr<Portal> otherPortal = portal->otherPortal.lock();
      if(!otherPortal) continue;
      // 2. ポータルのステンシルを書き込む
      renderer->getDeviceContext()->OMSetDepthStencilState(stencilStateIncr, static_cast<UINT>(level));
      portal->draw();
      // 3. ポータル面の深度を消す
      renderer->getDeviceContext()->OMSetDepthStencilState(stencilStateClearDepth, static_cast<UINT>(level + 1));
      drawFullscreenQuad();
      // 4. ポータルの先を表示
      auto cameraTransform = camera->transform;
      
      // 入口の向きから出口の向きの反対までの差rotを用意
      // カメラ位置 (入口ポータルからの相対) と向きをrotで変換し
      // それを出口のカメラ位置 (出口ポータルからの相対) と向きにする
      Near::Math::Quaternion portalInv;
      portal->transform.rotation.Inverse(portalInv);
      // 反対 = ローカルY軸で180度回転 (仮; 天井ポータルで動く気しない)
      Near::Math::Quaternion otherPortalInvOpposite = Near::Math::Quaternion::CreateFromYawPitchRoll(DirectX::XM_PI, 0, 0) * otherPortal->transform.rotation;
      // AからBの差 = B * ivnerse(A)
      // 入口から出口の差 = 反対(出口) * inverse(入口)
      Near::Math::Quaternion rot = otherPortalInvOpposite * portalInv;

      camera->transform.position -= portal->transform.position;
      camera->transform.position = Near::Math::Vector3::Transform(camera->transform.position, rot);
      camera->transform.rotation *= rot;
      camera->transform.position += otherPortal->transform.position;
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

void PortalScene::drawFullscreenQuad(){
  auto* renderer = Near::renderer();
  renderer->pushWorldTransform();
  renderer->setWorldTransform(Near::Math::Matrix::Identity);
  renderer->setViewTransform(Near::Math::Matrix::Identity);
  renderer->setProjectionTransform(Near::Math::Matrix::Identity);
  fullscreenQuad.draw(sizeof(Near::Vertex3D), 0, 4, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  renderer->popWorldTransform();
}
