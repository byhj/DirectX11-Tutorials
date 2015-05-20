
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
	 float4 Pos: POSITION;
	 float2 Tex : TEXCOORD0;
	 float3 Normal: NORMAL;
};

struct VS_OUT
{
	float4 Pos: SV_POSITION;
	float2 Tex :TEXCOORD0;
	float3 Normal: NORMAL;
	float3 viewDir: TEXCOORD1;
};

VS_OUT VS(VS_IN vs_in)
{
	float4 worldPos;

	VS_OUT vs_out;
	vs_in.Pos.w = 1.0f;

	vs_out.Pos = mul(vs_in.Pos, World);
	vs_out.Pos = mul(vs_out.Pos, View);
	vs_out.Pos = mul(vs_out.Pos, Proj);
	vs_out.Normal   = mul(vs_in.Normal, (float3x3)World);
	vs_out.Normal   = normalize(vs_out.Normal);

	float4 WorldPos = mul(vs_in.Pos, World);
	vs_out.viewDir = camPos.xyz - WorldPos.xyz;
	vs_out.viewDir = normalize(vs_out.viewDir);

	vs_out.Tex = vs_in.Tex;

	return vs_out;
}

