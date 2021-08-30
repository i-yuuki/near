#include "pch.h"
#include "utils.h"

#include <string>

namespace Near{

void throwResult(const char* msg, HRESULT res){
  LPWSTR errorText = nullptr;
  DWORD formatRes = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, res, 0, reinterpret_cast<LPWSTR>(&errorText), 0, nullptr);
  std::string exMsg = msg;
  exMsg.append(" (HRESULT ").append(std::to_string(res));
  if(SUCCEEDED(formatRes)){
    exMsg.append(" ").append(narrow(errorText)).append(")");
  }else{
    exMsg.append(" Unknown error)");
  }
  if(errorText){
    LocalFree(errorText);
  }
  throw std::exception(exMsg.c_str());
}

std::string narrow(std::wstring_view str){
  int u8Len = WideCharToMultiByte(CP_UTF8, 0, str.data(), str.size(), nullptr, 0, NULL, false);
  std::string u8;
  u8.resize(u8Len);
  WideCharToMultiByte(CP_UTF8, 0, str.data(), str.size(), u8.data(), u8Len, NULL, false);
  return u8;
}

std::wstring widen(std::string_view str){
  int wideLen = MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), nullptr, 0);
  std::wstring wide;
  wide.resize(wideLen);
  MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), wide.data(), wideLen);
  return wide;
}

Math::Quaternion createEularRotation(const Math::Vector3& degrees){
  auto radians = degrees * (DirectX::XM_PI / 180.0f);
  return Math::Quaternion::CreateFromYawPitchRoll(radians.y, radians.x, radians.z);
}

Math::Matrix createObliqueMatrix(const Math::Matrix& view, const Math::Matrix& projection, Math::Plane c){
  // http://www.terathon.com/lengyel/Lengyel-Oblique.pdf
  // M' =        M₁
  //             M₂
  //       2M₄ ･ Q             m3a
  //       ------- C - M₄  →  --- C - m₄
  //        C ･ Q              m3b
  //             M₄
  // Q = inverse(M)Q'
  // Q' = (sgn(C'x), sgn(C'y), 1, 1)
  // C' = transpose(inverse(M)) C
  // printf_s("Cw: %.3f\n", c.w);
  // Math::Matrix result = m;
  // Math::Matrix mInv;
  // m.Invert(mInv);
  // Math::Vector4 cDash = Math::Plane::Transform(c, mInv.Transpose());
  // Math::Vector4 qDash(sign(cDash.x), sign(cDash.y), 1, 1);
  // Math::Vector4 q = Math::Vector4::Transform(qDash, mInv);
  // Math::Vector4 m4(&m._41);
  // float m3a = (2 * m4).Dot(q);
  // float m3b = c.Dot(q);
  // Math::Vector4 m3 = (m3a / m3b) * Math::Vector4(c) - m4;
  // result._31 = m3.x;
  // result._32 = m3.y;
  // result._33 = m3.z;
  // result._34 = m3.w;

  // http://web.archive.org/web/20031221172114/http://developer.nvidia.com/object/oblique_frustum_clipping.html
  c = Near::Math::Plane::Transform(c, (view * projection).Invert().Transpose());
  c.Normalize();
  Math::Matrix result = Math::Matrix::Identity;
  result._13 = c.x;
  result._23 = c.y;
  result._33 = c.z;
  result._43 = c.w;
  result = projection * result;

  /*
  result.m[3][0] = 0;
  result.m[3][1] = 0;
  result.m[3][2] = -1;
  result.m[3][3] = 0;

  Math::Vector4 m3 = Math::Vector4((-2 * q) / (c * q) * c) + Math::Vector4(0, 0, 1, 0);
  result.m[2][0] = m3.x;
  result.m[2][1] = m3.y;
  result.m[2][2] = m3.z;
  result.m[2][3] = m3.w;
  */

  return result;
  
}

/*

Math::Vector3 toEulerAngles(const Math::Quaternion& q){
  Math::Vector3 angles;

  // roll (x-axis rotation)
  double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
  double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
  angles.x = std::atan2(sinr_cosp, cosr_cosp);

  // pitch (y-axis rotation)
  double sinp = 2 * (q.w * q.y - q.z * q.x);
  if (std::abs(sinp) >= 1)
      angles.y = std::copysign(DirectX::XM_PI / 2, sinp); // use 90 degrees if out of range
  else
      angles.y = std::asin(sinp);

  // yaw (z-axis rotation)
  double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
  double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
  angles.z = std::atan2(siny_cosp, cosy_cosp);

  return angles;
}

Math::Quaternion opposite(const Math::Quaternion& q){
  Math::Vector3 angles = -toEulerAngles(q);
  return Math::Quaternion::CreateFromYawPitchRoll(angles.z, angles.y, angles.x);
}

*/

_Use_decl_annotations_
void XM_CALLCONV createBoundingFrustum(DirectX::BoundingFrustum& Out, DirectX::FXMMATRIX Projection, bool rhcoords) noexcept{
  using namespace DirectX;
  // Corners of the projection frustum in homogenous space.
  static XMVECTORF32 HomogenousPoints[6] = {
    { { {  1.0f,  0.0f, 1.0f, 1.0f } } },   // right (at far plane)
    { { { -1.0f,  0.0f, 1.0f, 1.0f } } },   // left
    { { {  0.0f,  1.0f, 1.0f, 1.0f } } },   // top
    { { {  0.0f, -1.0f, 1.0f, 1.0f } } },   // bottom

    { { { 0.0f, 0.0f, 0.0f, 1.0f } } },     // near
    { { { 0.0f, 0.0f, 1.0f, 1.0f } } }      // far
  };

  XMVECTOR Determinant;
  XMMATRIX matInverse = XMMatrixInverse(&Determinant, Projection);

  // Compute the frustum corners in world space.
  XMVECTOR Points[6];

  for(size_t i = 0;i < 6;++ i){
    // Transform point.
    Points[i] = XMVector4Transform(HomogenousPoints[i], matInverse);
  }

  Out.Origin = XMFLOAT3(0.0f, 0.0f, 0.0f);
  Out.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

  // Compute the slopes.
  Points[0] = XMVectorMultiply(Points[0], XMVectorReciprocal(XMVectorSplatZ(Points[0])));
  Points[1] = XMVectorMultiply(Points[1], XMVectorReciprocal(XMVectorSplatZ(Points[1])));
  Points[2] = XMVectorMultiply(Points[2], XMVectorReciprocal(XMVectorSplatZ(Points[2])));
  Points[3] = XMVectorMultiply(Points[3], XMVectorReciprocal(XMVectorSplatZ(Points[3])));

  Out.RightSlope = XMVectorGetX(Points[0]);
  Out.LeftSlope = XMVectorGetX(Points[1]);
  Out.TopSlope = XMVectorGetY(Points[2]);
  Out.BottomSlope = XMVectorGetY(Points[3]);

  // Compute near and far.
  Points[4] = XMVectorMultiply(Points[4], XMVectorReciprocal(XMVectorSplatW(Points[4])));
  Points[5] = XMVectorMultiply(Points[5], XMVectorReciprocal(XMVectorSplatW(Points[5])));

  if(rhcoords){
    Out.Near = XMVectorGetZ(Points[5]);
    Out.Far = XMVectorGetZ(Points[4]);
  }else{
    Out.Near = XMVectorGetZ(Points[4]);
    Out.Far = XMVectorGetZ(Points[5]);
  }
}

}
