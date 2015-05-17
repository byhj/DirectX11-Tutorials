
//Notice the order of matrix member

cbuffer MatrixBuffer
{
	matrix World;
	matrix View;
	matrix Proj;
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
	vs_out.Position.w = 1.0f;
<<<<<<< HEAD
=======

>>>>>>> origin
	vs_out.Position = mul(vs_in.Position, World);
	vs_out.Position = mul(vs_out.Position, View);
	vs_out.Position = mul(vs_out.Position, Proj);

	vs_out.Color =  vs_in.Color;

	return vs_out;
}

