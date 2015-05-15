struct VS_OUT
{
	float4 Position: SV_POSITION;
	float4 Color: COLOR;
};


float4 PS(VS_OUT input): SV_TARGET
{
	return input.Color;
}