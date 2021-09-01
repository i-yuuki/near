#include "../nearlib/shaders/common.hlsl"

Texture2D tex0 : register(t0);
SamplerState samplerState : register(s0);

struct LevelPSOutput{
  float4 color : SV_Target0;
  float4 normal : SV_Target1;
};

void main(in GeneralPSInput input, out LevelPSOutput output){
  output.color = tex0.Sample(samplerState, input.uv) * input.color;
  output.normal = input.normal;
}
