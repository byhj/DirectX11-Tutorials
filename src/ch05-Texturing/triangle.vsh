
struct MatrixBuffer
{
   float4x4 model;
   float4x4 view;
   float4x4 proj;
};

cbuffer cbMatrix : register(b0)
{
	MatrixBuffer g_Mat;
};

struct VS_IN
{
   float4 Pos : POSITION0;
   float4 Tex : TEXCOORD0;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float4 Tex : TEXCOORD0;
};

VS_OUT VS( VS_IN vs_in )
{	
 
   VS_OUT vs_out;
   vs_out.Pos = mul(vs_in.Pos, g_Mat.model);
   vs_out.Pos = mul(vs_out.Pos, g_Mat.view);
   vs_out.Pos = mul(vs_out.Pos, g_Mat.proj);

   vs_out.Tex = vs_in.Tex;
 
   return vs_out;
}
