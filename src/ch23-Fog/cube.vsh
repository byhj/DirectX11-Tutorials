
cbuffer MatrixObject : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
};

 cbuffer FogBuffer	: register(b1)
{
    float fogStart;
    float fogEnd;
};

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
	float  fogFactor : FOG;
};

VS_OUT VS(VS_IN vs_in)
{	
 
   VS_OUT vs_out;
   vs_in.Pos.w = 1.0f;

  // Make the position with mvp matrix
   vs_out.Pos = mul(vs_in.Pos, model);
   vs_out.Pos = mul(vs_out.Pos, view);
   vs_out.Pos = mul(vs_out.Pos, proj);

   vs_out.Tex = vs_in.Tex;
   vs_out.Normal = mul(vs_in.Normal, (float3x3)model );
   vs_out.Normal = normalize(vs_out.Normal);
 
   // Calculate linear fog.   
   float4 cameraPosition = mul(vs_in.Pos, model);
          cameraPosition = mul(cameraPosition, view);

   vs_out.fogFactor = saturate((fogEnd - cameraPosition.z) / (fogEnd - fogStart));
 
   return vs_out;
}
