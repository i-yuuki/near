#include "common.hlsl"

cbuffer worldBuffer : register(b0){ matrix worldTransform; }
cbuffer viewBuffer : register(b1){ matrix viewTransform; }
cbuffer projectionBuffer : register(b2){ matrix projectionTransform; }

void main(in GeneralVSInput input, out GeneralPSInput output){
  output.position = input.position;
  output.position = mul(output.position, worldTransform);
  output.position = mul(output.position, viewTransform);
  output.position = mul(output.position, projectionTransform);

  output.color = input.color;
  float3 normal = normalize(mul(input.normal.xyz, worldTransform));
  output.normal = float4(normal.x, normal.y, normal.z, 1);
  // output.normal = normalize(mul(input.normal, worldTransform));
  // output.color = float4(normalize(input.normal.xyz), 1);
  output.uv = input.uv;
}
