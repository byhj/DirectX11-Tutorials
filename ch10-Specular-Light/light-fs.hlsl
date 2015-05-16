
cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

struct VS_OUT
{
	float4 Position: SV_POSITION;
	float2 Tex: TEXCOORD;
	float3 Normal: NORMAL;
};


Texture2D shaderTexture;
SamplerState SampleType;

float4 PS(VS_OUT input): SV_TARGET
{
	
	float4 textureColor = shaderTexture.Sample(SampleType, input.Tex);
	float3 lightDir = -lightDirection;

	float lightIntensity = saturate( dot(input.Normal, lightDir) );
	float4 color = ambientColor;
	if (lightIntensity > 0.0f)
	{
		color += saturate(diffuseColor * lightIntensity);
	}

	//Clamps the specified value within the range of 0 to 1.
	color = saturate(color);

	return color * textureColor;
}