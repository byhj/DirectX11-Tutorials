
cbuffer MatrixBuffer
{
	matrix World;
	matrix View;
	matrix Proj;

};

struct VS_IN
{
	 float4 Position  :POSITION;
};

struct VS_OUT
{
	 float4 Position  :SV_POSITION;
	  float4 depthPosition : TEXTURE0;
};

VS_OUT VS(VS_IN vs_in)
{
	VS_OUT vs_out;

	vs_in.Position.w = 1.0f;
	vs_out.Position = mul(vs_in.Position, World);
	vs_out.Position = mul(vs_out.Position, View);
	vs_out.Position = mul(vs_out.Position, Proj);
	
	// Store the position value in a second input value for depth value calculations.
	vs_out.depthPosition = vs_out.Position;
	
	return vs_out;
}

