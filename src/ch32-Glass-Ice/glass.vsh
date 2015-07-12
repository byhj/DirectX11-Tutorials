
struct MatrixBuffer
{
   float4x4 model;
   float4x4 proj;
   float4x4 view;
};

cbuffer cbMatirxBuffer : register(b0)
{
   MatrixBuffer cbMatrix;
};
  
struct VS_IN
{
   float4 Pos : POSITION;
   float2 Tex : TEXCOORD0;
};

struct VS_OUT
{
  float4 Pos :SV_POSITION;
  float2 Tex :TEXCOORD0;
  float4 RefractionPos: TEXCOORD1;
};

VS_OUT VS(VS_IN vs_in)
{
   vs_in.Pos.w = 1.0f;
   vs_in.Pos.z = 0.0f;
   VS_OUT vs_out;

   float4x4 mvp = mul(cbMatrix.view, cbMatrix.proj);
   mvp =  mul(cbMatrix.model, mvp);

   vs_out.Pos = mul(vs_in.Pos,  cbMatrix.model);
   vs_out.Pos = mul(vs_out.Pos, cbMatrix.view);
   vs_out.Pos = mul(vs_out.Pos, cbMatrix.proj);

   vs_out.Tex = vs_in.Tex;
   vs_out.RefractionPos = mul(vs_in.Pos, mvp);

   return vs_out;
}