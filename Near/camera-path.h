#pragma once

#include <string>
#include <NearLib/math.h>

class CameraPath{
public:
  struct BezierPoint{
    Near::Math::Vector3 p1;
    Near::Math::Vector3 p2;
    Near::Math::Vector3 p3;
    float time;
  };
  CameraPath();
  void reset(const Near::Math::Vector3& startPoint = Near::Math::Vector3::Zero);
  void load(const std::string& filePath);
  const std::vector<BezierPoint>& getPoints();
  void addPoint(const Near::Math::Vector3& prevPointHandle, const Near::Math::Vector3& point, const Near::Math::Vector3& handle, float duration);
  void addPointRelative(const Near::Math::Vector3& prevPointHandle, const Near::Math::Vector3& pointRelative, const Near::Math::Vector3& handle, float duration);
  void advance(float deltaTime);
  const Near::Math::Vector3& getStartPosition();
  const Near::Math::Vector3& getPosition();
  Near::Math::Vector3 getMovement();
  static Near::Math::Vector3 Evaluate(float t, const Near::Math::Vector3& p0, const Near::Math::Vector3& p1, const Near::Math::Vector3& p2, const Near::Math::Vector3& p3);
private:
  std::vector<BezierPoint> points;
  float time;
  Near::Math::Vector3 position;
  Near::Math::Vector3 lastPosition;
};
