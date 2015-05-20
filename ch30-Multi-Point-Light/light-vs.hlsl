#define NUM_LIGHTS 4

cbuffer MatrixBuffer
{
	matrix World;
	matrix View;
	matrix Proj;

};

cbuffer CameraBuffer
{
	float3 camPos;
	float padding;
};

cbuffer LightPositionBuffer
{
	float4 lightPosition[NUM_LIGHTS];
};

struct VS_IN
{
	 float4 Position: POSITION;
	 float2 Tex : TEXCOORD0;
	 float3 Normal: NORMAL;
};

struct VS_OUT
{
	float4 Position: SV_POSITION;
	float2 Tex :TEXCOORD0;
	float3 Normal: NORMAL;

    float3 lightPos1:TEXCOORD1;
	float3 lightPos2:TEXCOORD2;
	float3 lightPos3:TEXCOORD3;
	float3 lightPos4:TEXCOORD4;
};

VS_OUT VS(VS_IN vs_in)
{
	float4 worldPos;

	VS_OUT vs_out;
	vs_in.Position.w = 1.0f;

	vs_out.Position = mul(vs_in.Position, World);
	vs_out.Position = mul(vs_out.Position, View);
	vs_out.Position = mul(vs_out.Position, Proj);
	vs_out.Normal   = mul(vs_in.Normal, (float3x3)World);
	vs_out.Normal   = normalize(vs_out.Normal);

	float4 worldPosition = mul(vs_in.Position, World);
    // Determine the light positions based on the position of the lights and the position of the vertex in the world.
   vs_out.lightPos1.xyz = lightPosition[0].xyz - worldPosition.xyz;
   vs_out.lightPos2.xyz = lightPosition[1].xyz - worldPosition.xyz;
   vs_out.lightPos3.xyz = lightPosition[2].xyz - worldPosition.xyz;
   vs_out.lightPos4.xyz = lightPosition[3].xyz - worldPosition.xyz;

    // Normalize the light position vectors.
   vs_out.lightPos1 = normalize(vs_out.lightPos1);
   vs_out.lightPos2 = normalize(vs_out.lightPos2);
   vs_out.lightPos3 = normalize(vs_out.lightPos3);
   vs_out.lightPos4 = normalize(vs_out.lightPos4);

	vs_out.Tex = vs_in.Tex;

	return vs_out;
}

