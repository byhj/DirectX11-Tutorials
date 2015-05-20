
cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

struct VS_OUT
{
	float4 Position: SV_POSITION;
	float2 Tex: TEXCOORD;
	float3 Normal: NORMAL;
	float4 depthPosition:TEXTURE1;
};


Texture2D shaderTexture;
SamplerState SampleType;

float4 PS(VS_OUT vs_out): SV_TARGET
{
	
	float depthValue;
	float4 color;
	depthValue = vs_out.depthPosition.z / vs_out.depthPosition.w;
	if (depthValue < 0.9f)
		color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	if (depthValue > 0.9f)
		color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	if (depthValue > 0.925f)
		color  = float4(0.0f, 0.0f, 1.0f, 1.0f);

	return color;

}