#include "../nearlib/shaders/common.hlsl"

void main(in GeneralPSInput input, out float4 output : SV_Target){
  output = input.color;
}
