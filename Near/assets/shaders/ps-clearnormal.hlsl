#include "../nearlib/shaders/common.hlsl"

struct PSOutput{
  float4 color : SV_Target0;
  float4 normal : SV_Target1;
};

void main(in GeneralPSInput input, out PSOutput output){
  output.color = input.color;
  output.normal = float4(0, 0, 0, 0);
}
