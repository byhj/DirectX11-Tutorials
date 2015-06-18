
cbuffer MatrixObject
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
};

cbuffer CameraBuffer
{
  float3 camPos;
  float  padding;
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
	float3 ViewDir: TEXCOORD1;
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

   //Calc the view direction
   float4 worldPos = mul(vs_in.Pos, model);
   vs_out.ViewDir  = camPos.xyz - worldPos.xyz;
   vs_out.ViewDir  = normalize(vs_out.ViewDir);
    
   return vs_out;
}
