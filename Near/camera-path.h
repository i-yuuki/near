#pragma once

#include <string>
#include <NearLib/math.h>

class CameraPath{
public:
  CameraPath();
  void reset(const Near::Math::Vector3& startPoint = Near::Math::Vector3::Zero);
  void load(const std::string& filePath);
  void addPoint(const Near::Math::Vector3& prevPointHandle, const Near::Math::Vector3& point, const Near::Math::Vector3& handle, float duration);
  void addPointRelative(const Near::Math::Vector3& prevPointHandle, const Near::Math::Vector3& pointRelative, const Near::Math::Vector3& handle, float duration);
  void advance(float deltaTime);
  const Near::Math::Vector3& getPosition();
  Near::Math::Vector3 getMovement();
private:
  struct BezierPoint{
    Near::Math::Vector3 p1;
    Near::Math::Vector3 p2;
    Near::Math::Vector3 p3;
    float time;
  };
  std::vector<BezierPoint> points;
  float time;
  Near::Math::Vector3 position;
  Near::Math::Vector3 lastPosition;
};
