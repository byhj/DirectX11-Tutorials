
cbuffer MatrixBuffer
{
	matrix World;
	matrix View;
	matrix Proj;

};

struct VS_IN
{
	 float4 Pos: POSITION;
	 float2 Tex : TEXCOORD;
};

struct VS_OUT
{
	float4 Pos: SV_POSITION;
	float2 Tex :TEXCOORD;
};

VS_OUT VS(VS_IN vs_in)
{
	VS_OUT vs_out;
	vs_in.Pos.w = 1.0f;
	vs_out.Pos = mul(vs_in.Pos, World);
	vs_out.Pos = mul(vs_out.Pos, View);
	vs_out.Pos = mul(vs_out.Pos, Proj);

	vs_out.Tex = vs_in.Tex;

	return vs_out;
}

