#include "camera-path.h"

#include <algorithm>

CameraPath::CameraPath(){
}

void CameraPath::reset(const Near::Math::Vector3& startPoint){
  points.clear();
  BezierPoint point{};
  point.p3 = startPoint;
  points.push_back(point);
  points.shrink_to_fit();
  time = 0;
  position = startPoint;
  lastPosition = startPoint;
}

void CameraPath::addPoint(const Near::Math::Vector3& prevPointHandle, const Near::Math::Vector3& point, const Near::Math::Vector3& handle, float duration){
  auto prevPoint = points.rbegin();
  points.push_back({prevPoint->p3 + prevPointHandle, point + handle, point, prevPoint->time + duration});
}

void CameraPath::addPointRelative(const Near::Math::Vector3& prevPointHandle, const Near::Math::Vector3& pointRelative, const Near::Math::Vector3& handle, float duration){
  auto prevPoint = points.rbegin();
  addPoint(prevPointHandle, prevPoint->p3 + pointRelative, handle, duration);
}

void CameraPath::advance(float deltaTime){
  if(points.empty()) return;

  lastPosition = position;
  time = std::fmod(time + deltaTime, points.rbegin()->time);

  BezierPoint pointToFind{};
  // timeだけ比較、他の値はなんでもいいので設定しない
  pointToFind.time = time;
  // point.time >= time になる最初の点を探す
  auto it = std::lower_bound(points.begin(), points.end(), pointToFind, [](const BezierPoint& a, const BezierPoint& b){
    return a.time < b.time;
  });

  if(it == points.end()) return; // ??????

  BezierPoint pointZero{};
  bool isFirstSegment = it == points.begin();
  const auto& prevPoint = isFirstSegment ? pointZero : *(it - 1);

  float segmentStartTime = prevPoint.time;
  float segmentDuration = it->time - segmentStartTime;
  float t = (time - segmentStartTime) / segmentDuration;
  // 線形補間でベジエ (De Casteljau's algorithm)
  // 制御点を用意
  const auto& p0 = prevPoint.p3;
  // 各制御点をつなぐ
  Near::Math::Vector3 a = Near::Math::Vector3::Lerp(p0,     it->p1, t);
  Near::Math::Vector3 b = Near::Math::Vector3::Lerp(it->p1, it->p2, t);
  Near::Math::Vector3 c = Near::Math::Vector3::Lerp(it->p2, it->p3, t);
  // つないだものをさらにつなぐ
  Near::Math::Vector3 d = Near::Math::Vector3::Lerp(a, b, t);
  Near::Math::Vector3 e = Near::Math::Vector3::Lerp(b, c, t);
  // またまたつないで完成
  position = Near::Math::Vector3::Lerp(d, e, t);
  printf("%.3f - %.3f / %.3f / %.3f\n", time, position.x, position.y, position.z);
}

Near::Math::Vector3 CameraPath::getMovement(){
  return position - lastPosition;
}