
cbuffer PixelBuffer
{
	float4 pixelColor;
};

struct VS_OUT
{
	float4 Pos: SV_POSITION;
	float2 Tex: TEXCOORD;
};

Texture2D shaderTexture;
SamplerState SampleType;

float4 PS(VS_OUT input): SV_TARGET
{
	float4 color = shaderTexture.Sample(SampleType, input.Tex);

	    // If the color is black on the texture then treat this pixel as transparent.
    if(color.r == 0.0f)
    {
        color.a = 0.0f;
    }
	
    // If the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color.
    else
    {
        color.a = 1.0f;
        color = color * pixelColor;
    }

	return color;
}