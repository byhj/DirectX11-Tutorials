
cbuffer MatrixBuffer
{
	matrix World;
	matrix Proj;
	matrix View;

};

struct VS_IN
{
	 float4 Position: POSITION;
	 float4 Color : COLOR;
};

struct VS_OUT
{
	float4 Position: SV_POSITION;
	float4 Color: COLOR;
};

VS_OUT VS(VS_IN vs_in)
{
	VS_OUT vs_out;
	vs_out.Position = vs_in.Position;
	vs_out.Color =  vs_in.Color;

	return vs_out;
}

