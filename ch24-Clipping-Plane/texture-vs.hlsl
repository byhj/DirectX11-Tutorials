
cbuffer MatrixBuffer
{
	matrix World;
	matrix View;
	matrix Proj;

};

cbuffer CameraBuffer
{
	float3 cameraPosition;
};

cbuffer ClipPlaneBuffer
{
    float4 clipPlane;
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
	 float4 Position  :SV_POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 normal    :NORMAL;
	 float3 tangent   :TANGENT;
	 float3 binormal  :BINORMAL;
	 float3 viewDirection :TEXCOORD1;

	  float clip : SV_ClipDistance0;
};

VS_OUT VS(VS_IN vs_in)
{
	VS_OUT vs_out;
	float4 worldPosition;

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

	//We need view direction to make specular light
	worldPosition = mul(vs_in.Position, World);
	vs_out.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	vs_out.viewDirection = normalize(vs_out.viewDirection);
	    // Set the clipping plane.
    vs_out.clip = dot(mul(vs_in.Position, World), clipPlane);

	return vs_out;
}

