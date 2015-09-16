
cbuffer MatrixObject : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
};

cbuffer ClipBuffer  : register(b1)
{
   float4 clipPlane;
}

struct VS_IN
{
   float4 Pos: POSITION; 
   float2 Tex: TEXCOORD0;
   float3 Normal: NORMAL;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
	float3 Normal: NORMAL;
    float  Clip : SV_ClipDistance0;
};

VS_OUT VS(VS_IN vs_in)
{	
 
   VS_OUT vs_out;
  // Make the position with mvp matrix
   vs_out.Pos = mul(vs_in.Pos, model);
   vs_out.Pos = mul(vs_out.Pos, view);
   vs_out.Pos = mul(vs_out.Pos, proj);

   vs_out.Tex = vs_in.Tex;
   vs_out.Normal = mul(vs_in.Normal, (float3x3)model );
   vs_out.Normal = normalize(vs_out.Normal);

   // Set the clipping plane, clip if < 0
   // n*p + d > 0
   vs_out.Clip = dot(mul(vs_in.Pos, model), clipPlane);

   return vs_out;
}
