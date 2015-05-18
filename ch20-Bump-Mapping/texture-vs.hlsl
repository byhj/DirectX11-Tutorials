
cbuffer MatrixBuffer
{
	matrix World;
	matrix View;
	matrix Proj;

};

struct VS_IN
{
	 float4 Position  :POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 normal    :NORMAL;
	 float3 tangent   :TANGENT;
	 float3 binormal  :BINORMAL;
};

struct VS_OUT
{
	 float4 Position  :POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 normal    :NORMAL;
	 float3 tangent   :TANGENT;
	 float3 binormal  :BINORMAL;
};

VS_OUT VS(VS_IN vs_in)
{
	VS_OUT vs_out;
	vs_in.Position.w = 1.0f;

	vs_out.Position = mul(vs_in.Position, World);
	vs_out.Position = mul(vs_out.Position, View);
	vs_out.Position = mul(vs_out.Position, Proj);

	vs_out.Tex = vs_in.Tex;
    // Calculate the normal vector against the world matrix only and then normalize the final value.
    vs_out.normal = mul(vs_in.normal, (float3x3)World);
    vs_out.normal = normalize(vs_out.normal);

	vs_out.tangent = mul(vs_in.tangent, (float3x3)World);
    vs_out.tangent = normalize(vs_out.tangent);
	
	vs_out.binormal = mul(vs_in.binormal, (float3x3)World);
    vs_out.binormal = normalize(vs_out.binormal);

	return vs_out;
}

