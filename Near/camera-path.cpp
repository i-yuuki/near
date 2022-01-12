#include "camera-path.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <NearLib/utils.h>

CameraPath::CameraPath(){
}

void CameraPath::reset(const Near::Math::Vector3& startPoint, const Near::Math::Quaternion& startRotation){
  points.clear();
  BezierPoint point{};
  point.p3 = startPoint;
  points.push_back(point);
  points.shrink_to_fit();
  rotations.clear();
  RotationPoint rotPoint{};
  rotPoint.rotation = startRotation;
  rotations.push_back(rotPoint);
  rotations.shrink_to_fit();
  time = 0;
  position = startPoint;
  lastPosition = startPoint;
  rotation = startRotation;
}

void CameraPath::load(const std::string& filePath){
  std::ifstream f(filePath);
  if(!f) throw std::exception("File open failed");

  Near::Math::Vector3 startPoint;
  Near::Math::Vector3 startRotation;
  f >> startPoint.x >> startPoint.y >> startPoint.z;
  f >> startRotation.x >> startRotation.y >> startRotation.z;
  reset(startPoint, Near::createEularRotation(startRotation));

  while(true){
    std::string word;
    f >> word;
    if(!f) break;
    if(word == u8"pos"){
      f >> word;
      bool isRelative = word == u8"rel";
      float duration;
      Near::Math::Vector3 prevPointHandle, point, handle;
      f >> duration;
      f >> prevPointHandle.x >> prevPointHandle.y >> prevPointHandle.z;
      f >> point.x >> point.y >> point.z;
      f >> handle.x >> handle.y >> handle.z;
      if(isRelative){
        addPointRelative(prevPointHandle, point, handle, duration);
      }else{
        addPoint(prevPointHandle, point, handle, duration);
      }
    }else if(word == u8"rot"){
      f >> word;
      float duration;
      Near::Math::Vector3 rotation;
      f >> duration;
      f >> rotation.x >> rotation.y >> rotation.z;
      addRotation(Near::createEularRotation(rotation), duration);
    }
  }
}

const std::vector<CameraPath::BezierPoint>& CameraPath::getPoints(){
  return points;
}

void CameraPath::addPoint(const Near::Math::Vector3& prevPointHandle, const Near::Math::Vector3& point, const Near::Math::Vector3& handle, float duration){
  auto prevPoint = points.rbegin();
  points.push_back({prevPoint->p3 + prevPointHandle, point + handle, point, prevPoint->time + duration});
}

void CameraPath::addPointRelative(const Near::Math::Vector3& prevPointHandle, const Near::Math::Vector3& pointRelative, const Near::Math::Vector3& handle, float duration){
  auto prevPoint = points.rbegin();
  addPoint(prevPointHandle, prevPoint->p3 + pointRelative, handle, duration);
}

void CameraPath::addRotation(const Near::Math::Quaternion& rotation, float duration){
  auto prevPoint = rotations.rbegin();
  rotations.push_back({rotation, prevPoint->time + duration});
}

void CameraPath::advance(float deltaTime){
  if(points.empty()) return;

  lastPosition = position;
  time = std::fmod(time + deltaTime, points.rbegin()->time);

  updatePosition();
  updateRotation();
}

void CameraPath::updatePosition(){
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
  position = Evaluate(t, prevPoint.p3, it->p1, it->p2, it->p3);
}

void CameraPath::updateRotation(){
  RotationPoint pointToFind{};
  pointToFind.time = time;
  auto it = std::lower_bound(rotations.begin(), rotations.end(), pointToFind, [](const RotationPoint& a, const RotationPoint& b){
    return a.time < b.time;
  });

  RotationPoint pointZero{};
  bool isFirstSegment = it == rotations.begin();
  const auto& prevPoint = isFirstSegment ? pointZero : *(it - 1);

  float segmentStartTime = prevPoint.time;
  float segmentDuration = it->time - segmentStartTime;
  float t = (time - segmentStartTime) / segmentDuration;
  // 仮置きeaseInOutQuad
  t = t < 0.5f ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2;
  rotation = Near::Math::Quaternion::Lerp(prevPoint.rotation, it->rotation, t);
}

const Near::Math::Vector3& CameraPath::getStartPosition(){
  return points[0].p3;
}

const Near::Math::Vector3& CameraPath::getPosition(){
  return position;
}

Near::Math::Vector3 CameraPath::getMovement(){
  return position - lastPosition;
}

const Near::Math::Quaternion& CameraPath::getRotation(){
  return rotation;
}

Near::Math::Vector3 CameraPath::Evaluate(float t, const Near::Math::Vector3& p0, const Near::Math::Vector3& p1, const Near::Math::Vector3& p2, const Near::Math::Vector3& p3){
  // 線形補間でベジエ (De Casteljau's algorithm)
  // 各制御点をつなぐ
  Near::Math::Vector3 a = Near::Math::Vector3::Lerp(p0, p1, t);
  Near::Math::Vector3 b = Near::Math::Vector3::Lerp(p1, p2, t);
  Near::Math::Vector3 c = Near::Math::Vector3::Lerp(p2, p3, t);
  // つないだものをさらにつなぐ
  Near::Math::Vector3 d = Near::Math::Vector3::Lerp(a, b, t);
  Near::Math::Vector3 e = Near::Math::Vector3::Lerp(b, c, t);
  // またまたつないで完成
  return Near::Math::Vector3::Lerp(d, e, t);
}
