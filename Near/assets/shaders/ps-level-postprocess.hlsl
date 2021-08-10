#include "../nearlib/shaders/common.hlsl"

Texture2D texColor : register(t0);
Texture2D texNormal : register(t1);
Texture2D texDepth : register(t2);
SamplerState samplerState : register(s0);

void main(in GeneralPSInput input, out float4 output : SV_Target){
  output = texColor.Sample(samplerState, input.uv);
  
  // https://williamchyr.com/development-update-edge-detection/
  // 画面ピクセルAのとき、BとEの法線、次にCとDの法線を比べる
  // +-+-+-+
  // |B| |C|
  // +-+-+-+
  // | |A| |
  // +-+-+-+
  // |D| |E|
  // +-+-+-+

  if(length(texNormal.Sample(samplerState, input.uv, int2(-1, -1)) - texNormal.Sample(samplerState, input.uv, int2(1, 1))) >= 1.0f){
    output = float4(0, 0, 0, 1);
  }else if(length(texNormal.Sample(samplerState, input.uv, int2(1, -1)) - texNormal.Sample(samplerState, input.uv, int2(-1, 1))) >= 1.0f){
    output = float4(0, 0, 0, 1);
  }else{
    output.a = 1;
  }
}
