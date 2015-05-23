
cbuffer MatrixBuffer
{
	matrix World;
	matrix View;
	matrix Proj;
	matrix LightView;
	matrix LightProj;
};

struct VS_IN
{
	 float4 Position  :POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 Normal    :NORMAL;
};

struct VS_OUT
{
	 float4 Position  :SV_POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 Normal    :NORMAL;
	 float4 ViewPos :TEXCOORD1;
};

VS_OUT VS(VS_IN vs_in)
{
	VS_OUT vs_out;
	float4 worldPosition;

	vs_in.Position.w = 1.0f;
	vs_out.Position = mul(vs_in.Position, World);
	vs_out.Position = mul(vs_out.Position, View);
	vs_out.Position = mul(vs_out.Position, Proj);
	    
	// Calculate the position of the vertice as viewed by the light source.
    vs_out.ViewPos = mul(vs_in.Position, World);
    vs_out.ViewPos = mul(vs_out.ViewPos, LightView);
    vs_out.ViewPos = mul(vs_out.ViewPos, LightProj);

	vs_out.Tex = vs_in.Tex;
    // Calculate the normal vector against the world matrix only and then normalize the final value.
    vs_out.Normal = mul(vs_in.Normal, (float3x3)World);
    vs_out.Normal = normalize(vs_out.Normal);

	return vs_out;
}

