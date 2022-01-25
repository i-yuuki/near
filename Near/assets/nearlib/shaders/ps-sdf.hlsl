#include "common.hlsl"

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);
cbuffer psBuffer : register(b1){
  float fontScale;
}

void main(in GeneralPSInput input, out float4 output : SV_Target){
  float dist = (tex.Sample(samplerState, input.uv).r - 0.5) * fontScale;
  output = input.color;
  output.a = smoothstep(-0.1, 0.1, dist);
}
