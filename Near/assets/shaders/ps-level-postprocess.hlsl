#include "../nearlib/shaders/common.hlsl"

Texture2D texColor : register(t0);
Texture2D texNormal : register(t1);
Texture2D texDepth : register(t2);
SamplerState samplerState : register(s0);

float linearDepth(float z, float near, float far){
  return near * far / (far + z * (near - far));
}

float linearDepth(float2 uv, int2 offset){
  // TODO? nearとfarを定数バッファに入れる
  return linearDepth(texDepth.Sample(samplerState, uv, offset).r, 0.1f, 8000);
}

void main(in GeneralPSInput input, out float4 output : SV_Target){
  output = texColor.Sample(samplerState, input.uv);
  
  // https://williamchyr.com/development-update-edge-detection/
  // 画面ピクセルAのとき、BとEの法線、次にCとDの法線を比べる
  // 全ての法線テストに落ちたらAの深度とBCDEの平均を比べる
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
    float surroundingDepth = linearDepth(input.uv, int2(-1, -1)).r
                           + linearDepth(input.uv, int2( 1, -1)).r
                           + linearDepth(input.uv, int2(-1,  1)).r
                           + linearDepth(input.uv, int2( 1,  1)).r;
    float depth = linearDepth(input.uv, 0).r;
    if(abs(surroundingDepth / 4 - depth) >= 1){
      output = float4(0, 0, 0, 1);
    }else{
      output.a = 1;
    }
  }
}
