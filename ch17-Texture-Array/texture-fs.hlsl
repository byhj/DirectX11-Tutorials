struct VS_OUT
{
	float4 Position: SV_POSITION;
	float2 Tex: TEXCOORD0;
};

Texture2D shaderTextures[2];
SamplerState SampleType;

float4 PS(VS_OUT input): SV_TARGET
{
	float4 color1, color2;
	color1 = shaderTextures[0].Sample(SampleType, input.Tex);
	color2 = shaderTextures[1].Sample(SampleType, input.Tex);

	float4 blendColor = color1 * color2 * 2.0f;
	blendColor = saturate(blendColor);

	return blendColor;
}