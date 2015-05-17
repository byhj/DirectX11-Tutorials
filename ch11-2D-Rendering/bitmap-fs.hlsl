struct VS_OUT
{
	float4 Position: SV_POSITION;
	float2 Tex: TEXCOORD;
};

Texture2D shaderTexture;
SamplerState SampleType;

float4 PS(VS_OUT input): SV_TARGET
{
	float4 textureColor = shaderTexture.Sample(SampleType, input.Tex);

	return textureColor;
}