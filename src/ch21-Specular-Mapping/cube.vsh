
cbuffer MatrixObject : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
};

cbuffer CameraBuffer  : register(b1)
{
  float4 camPos;
}

struct VS_IN
{
	 float4 pos       : POSITION;
	 float2 tex       : TEXCOORD0;
	 float3 normal    : NORMAL;
	 float3 tangent   : TANGENT;
	 float3 binormal  : BINORMAL;
};

struct VS_OUT
{
    float4 pos       : SV_POSITION;
    float2 tex       : TEXCOORD0;
	float3 normal    : NORMAL;
	float3 tangent   : TANGENT;
	float3 binormal  : BINORMAL;
	float3 viewDir   : TEXCOORD1;
};

VS_OUT VS(VS_IN vs_in)
{	
 
   VS_OUT vs_out;

  // Make the position with mvp matrix
   vs_out.pos = mul( vs_in.pos, model);
   vs_out.pos = mul(vs_out.pos, view);
   vs_out.pos = mul(vs_out.pos, proj);

   //Set the normal, tangent, binormal
   vs_out.normal    = mul(vs_in.normal, (float3x3)model );
   vs_out.normal    = normalize(vs_out.normal);

   vs_out.tangent   = mul(vs_in.tangent, (float3x3)model );
   vs_out.tangent   = normalize(vs_out.tangent);

	vs_out.binormal = mul(vs_in.binormal, (float3x3)model );
    vs_out.binormal = normalize(vs_out.binormal);
    vs_out.tex = vs_in.tex;

   //Calc the view direction
   float4 worldPos = mul(vs_in.pos, model);
   vs_out.viewDir  = camPos.xyz - worldPos.xyz;
   vs_out.viewDir  = normalize(vs_out.viewDir);
    
   return vs_out;
}
