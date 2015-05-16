struct VS_OUT
{
	float4 Position: SV_POSITION;
	float2 Tex: TEXCOORD0;
};


float4 PS(VS_OUT input): SV_TARGET
{
	return input.Color;
}