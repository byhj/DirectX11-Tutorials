
struct VS_OUT
{
	 float4 Position  :SV_POSITION;
	 float2 Tex       :TEXCOORD0;
	 float4 reflectionPosition : TEXCOORD1;
};


Texture2D shaderTexture;
Texture2D reflectionTexture;
SamplerState SampleType;

float4 PS(VS_OUT vs_out): SV_TARGET
{
	float4 textureColor;
    float2 reflectTexCoord;
    float4 reflectionColor;
    float4 color;


    textureColor = shaderTexture.Sample(SampleType, vs_out.Tex);
    
    // Calculate the projected reflection texture coordinates.
    reflectTexCoord.x = vs_out.reflectionPosition.x / vs_out.reflectionPosition.w / 2.0f + 0.5f;
    reflectTexCoord.y = -vs_out.reflectionPosition.y / vs_out.reflectionPosition.w / 2.0f + 0.5f;

	// Sample the texture pixel from the reflection texture using the projected texture coordinates.
    reflectionColor = reflectionTexture.Sample(SampleType, reflectTexCoord);

	// Do a linear interpolation between the two textures for a blend effect.
    color = lerp(textureColor, reflectionColor, 0.15f);

	return color;
}