#include "common.hlsl"

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);
// bool useTexture : register(b0);

float3 g_lightDirection = {0, -1, 0};
float3 g_materialAmbient = {0.4f, 0.4f, 0.4f};
float3 g_materialDiffuse = {0.6f, 0.6f, 0.6f};

void main(in GeneralPSInput input, out float4 output : SV_Target){
  float4 c = tex.Sample(samplerState, input.uv);
  // c.a = 1;
  c *= input.color;

  // output = input.color;

  // float brightness = max(-dot(normalize(input.normal.xyz), g_lightDirection), 0.0f);
  // c.rgb *= g_materialDiffuse * brightness;
  // c.rgb += g_materialAmbient;

  output = c;
  // if(useTexture){
  //   output = tex.Sample(samplerState, input.uv) * input.color;
  // }else{
  //   output = input.color;
  // }
}
