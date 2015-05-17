
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
	float3 viewDir: TEXCOORD1;
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

	float4 WorldPos = mul(vs_in.Position, World);
	vs_out.viewDir = camPos.xyz - WorldPos.xyz;
	vs_out.viewDir = normalize(vs_out.viewDir);

	vs_out.Tex = vs_in.Tex;

	return vs_out;
}

