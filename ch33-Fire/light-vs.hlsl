
cbuffer MatrixBuffer
{
	matrix World;
	matrix View;
	matrix Proj;

};

cbuffer NoiseBuffer
{
    float frameTime;
    float3 scrollSpeeds;
    float3 scales;
    float padding;
};

struct VS_IN
{
	 float4 Position: POSITION;
	 float2 Tex : TEXCOORD;
	 float3 Normal: NORMAL;
};

struct VS_OUT
{
	float4 Position: SV_POSITION;
	float2 Tex :TEXCOORD;
	float2 texCoords1 : TEXCOORD1;
    float2 texCoords2 : TEXCOORD2;
    float2 texCoords3 : TEXCOORD3;
};

VS_OUT VS(VS_IN vs_in)
{
	VS_OUT vs_out;
	vs_in.Position.w = 1.0f;

	vs_out.Position = mul(vs_in.Position, World);
	vs_out.Position = mul(vs_out.Position, View);
	vs_out.Position = mul(vs_out.Position, Proj);

	    // Compute texture coordinates for first noise texture using the first scale and upward scrolling speed values.
    vs_out.texCoords1 = (vs_in.Tex * scales.x);
    vs_out.texCoords1.y = vs_out.texCoords1.y + (frameTime * scrollSpeeds.x);

    // Compute texture coordinates for second noise texture using the second scale and upward scrolling speed values.
    vs_out.texCoords2 = (vs_in.Tex * scales.y);
    vs_out.texCoords2.y = vs_out.texCoords2.y + (frameTime * scrollSpeeds.y);

    // Compute texture coordinates for third noise texture using the third scale and upward scrolling speed values.
    vs_out.texCoords3 = (vs_in.Tex * scales.z);
    vs_out.texCoords3.y = vs_out.texCoords3.y + (frameTime * scrollSpeeds.z);


	vs_out.Tex = vs_in.Tex;

	return vs_out;
}

