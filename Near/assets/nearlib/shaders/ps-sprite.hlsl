#include "common.hlsl"

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);
cbuffer psBuffer : register(b0){
  bool useTexture;
}

void main(in GeneralPSInput input, out float4 output : SV_Target){
  output = useTexture ? tex.Sample(samplerState, input.uv) * input.color : input.color;
}
