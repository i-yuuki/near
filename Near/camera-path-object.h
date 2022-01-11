#pragma once

#include <NearLib/game-object.h>
#include <NearLib/buffer.h>

#include "camera-path.h"

class CameraPathObject : public Near::GameObject{
public:
  CameraPathObject(CameraPath* path);
  virtual void init(Near::Layer* layer) override;
  virtual void draw() override;
  virtual void uninit() override;
  void generatePath();
  bool isVisible();
  void setVisible(bool visible);
private:
  CameraPath* path;
  bool visible = false;
  Near::StandardVertexBuffer vertexBuffer;
  Near::IndexBuffer indexBuffer;
  std::shared_ptr<Near::VertexShader> vertexShader;
  std::shared_ptr<Near::PixelShader> pixelShader;
  static void GenerateVertices(const CameraPath::BezierPoint& from, const CameraPath::BezierPoint& to, std::vector<Near::Vertex3D>& vertices, std::vector<uint32_t>& indices);
};
