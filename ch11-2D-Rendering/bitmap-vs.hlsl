
cbuffer MatrixBuffer
{
	matrix World;
	matrix View;
	matrix Proj;

};

struct VS_IN
{
	 float4 Position: POSITION;
	 float2 Tex : TEXCOORD;
};

struct VS_OUT
{
	float4 Position: SV_POSITION;
	float2 Tex :TEXCOORD;
};

VS_OUT VS(VS_IN vs_in)
{
	VS_OUT vs_out;
	vs_in.Position.w = 1.0f;
	vs_out.Position = mul(vs_in.Position, World);
	vs_out.Position = mul(vs_out.Position, View);
	vs_out.Position = mul(vs_out.Position, Proj);

	vs_out.Tex = vs_in.Tex;

	return vs_out;
}

