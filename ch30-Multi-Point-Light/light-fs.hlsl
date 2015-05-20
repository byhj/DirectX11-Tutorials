#define NUM_LIGHTS 4

cbuffer LightColorBuffer
{
    float4 diffuseColor[NUM_LIGHTS];
};

struct VS_OUT
{
	float4 Position: SV_POSITION;
	float2 Tex: TEXCOORD0;
	float3 Normal: NORMAL;

	float3 lightPos1 : TEXCOORD1;
    float3 lightPos2 : TEXCOORD2;
    float3 lightPos3 : TEXCOORD3;
    float3 lightPos4 : TEXCOORD4;
};


Texture2D shaderTexture;
SamplerState SampleType;

float4 PS(VS_OUT input): SV_TARGET
{
	float lightIntensity1, lightIntensity2, lightIntensity3, lightIntensity4;
    float4 color, color1, color2, color3, color4;

    // Calculate the different amounts of light on this pixel based on the positions of the lights.
    lightIntensity1 = saturate(dot(input.Normal, input.lightPos1));
    lightIntensity2 = saturate(dot(input.Normal, input.lightPos2));
    lightIntensity3 = saturate(dot(input.Normal, input.lightPos3));
    lightIntensity4 = saturate(dot(input.Normal, input.lightPos4));
	color1 = diffuseColor[0] * lightIntensity1;
    color2 = diffuseColor[1] * lightIntensity2;
    color3 = diffuseColor[2] * lightIntensity3;
    color4 = diffuseColor[3] * lightIntensity4;

	float4 textureColor = shaderTexture.Sample(SampleType, input.Tex);
    color = saturate(color1 + color2 + color3 + color4) * textureColor;

	return color;
}