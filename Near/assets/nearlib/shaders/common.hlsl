
struct GeneralVSInput{
  float4 position : POSITION0;
  float4 normal : NORMAL0;
  float4 color : COLOR0;
  float2 uv : TEXCOORD0;
};

struct GeneralPSInput{
  float4 position : SV_POSITION;
  float4 normal : NORMAL0;
  float4 color : COLOR0;
  float2 uv : TEXCOORD0;
};
