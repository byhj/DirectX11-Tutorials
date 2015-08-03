
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

struct VS_OUTPUT 
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

VS_OUTPUT VS(float4 inPos: POSITION, float4 inColor: COLOR0)
{	
 
   VS_OUTPUT vs_out;
   vs_out.Pos = mul(inPos, g_Mat.model);
   vs_out.Pos = mul(vs_out.Pos, g_Mat.view);
   vs_out.Pos = mul(vs_out.Pos, g_Mat.proj);

   vs_out.Color = inColor;
 
   return vs_out;
}
