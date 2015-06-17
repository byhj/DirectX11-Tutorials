
cbuffer MatrixBuffer
{
	matrix proj;
	matrix view;
	matrix model;
};

struct VS_IN
{
  float4 Position: POSITION;
  float4 Color: COLOR0;
} ;

struct PS_IN
{
  float4 Position: SV_POSITION;
  float4 Color: COLOR0;
} ;

PS_IN VS(VS_IN vs_in)
{
   PS_IN vs_out;
   vs_in.Position.w = 1.0f;
   vs_out.Position = vs_in.Position;
   //vs_out.Position = mul(vs_in.Position, model);
  // vs_out.Position = mul(vs_out.Position, view);
   //vs_out.Position = mul(vs_out.Position, proj);
   vs_out.Color   =  vs_in.Color;

   return vs_out;
}