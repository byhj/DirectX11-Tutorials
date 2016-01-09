
cbuffer d3d::MatrixBuffer : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
	float4x4 lightView1;
	float4x4 lightProj1;
	float4x4 lightView2;
	float4x4 lightProj2;
};

cbuffer LightBuffer : register(b1)
{
  float4 LightPos1;
  float4 LightPos2;
};

struct VS_IN
{
	 float4 Pos       :POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 Normal    :NORMAL;
};

struct VS_OUT
{
    float4 Pos           : SV_POSITION;
    float2 Tex           : TEXCOORD0;
	float3 Normal        : NORMAL;
	float4 LightViewPos1 : TEXCOORD1;
	float3 LightPos1     : TEXCOORD2;
	float4 LightViewPos2 : TEXCOORD3;
	float3 LightPos2     : TEXCOORD4;
};

VS_OUT VS(VS_IN vs_in)
{	
 
   VS_OUT vs_out;
   vs_out.Pos.w = 1.0f;

  // Make the position with mvp matrix
   vs_out.Pos = mul(vs_in.Pos, model);
   vs_out.Pos = mul(vs_out.Pos, view);
   vs_out.Pos = mul(vs_out.Pos, proj);

   vs_out.LightViewPos1 = mul(vs_in.Pos, model);
   vs_out.LightViewPos1 = mul(vs_out.LightViewPos1, lightView1);
   vs_out.LightViewPos1 = mul(vs_out.LightViewPos1, lightProj1);

   vs_out.LightViewPos2 = mul(vs_in.Pos, model);
   vs_out.LightViewPos2 = mul(vs_out.LightViewPos2, lightView2);
   vs_out.LightViewPos2 = mul(vs_out.LightViewPos2, lightProj2);

   vs_out.Tex = vs_in.Tex;

   vs_out.Normal = mul(vs_in.Normal, (float3x3)model);
   vs_out.Normal = normalize(vs_out.Normal);

   float4 worldPos = mul(vs_in.Pos, model);
   vs_out.LightPos1 = LightPos1.xyz - worldPos.xyz;
   vs_out.LightPos1 = normalize(vs_out.LightPos1);

   vs_out.LightPos2 = LightPos2.xyz - worldPos.xyz;
   vs_out.LightPos2 = normalize(vs_out.LightPos2);
   return vs_out;
}
