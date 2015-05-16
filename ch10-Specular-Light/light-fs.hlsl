
cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
};

struct VS_OUT
{
	float4 Position: SV_POSITION;
	float2 Tex: TEXCOORD0;
	float3 Normal: NORMAL;
	float3 viewDir: TEXCOORD1;
};


Texture2D shaderTexture;
SamplerState SampleType;

float4 PS(VS_OUT input): SV_TARGET
{
	float3 reflection;
	float4 specular= float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 textureColor = shaderTexture.Sample(SampleType, input.Tex);
	float3 lightDir = -lightDirection;

	float lightIntensity = saturate( dot(input.Normal, lightDir) );
	float4 color = ambientColor;
	if (lightIntensity > 0.0f)
	{
		color += saturate(diffuseColor * lightIntensity);
		reflection = normalize(2 * lightIntensity * input.Normal - lightDir);
		specular = pow(saturate(dot(reflection, input.viewDir)), specularPower);
	}
	color = color * textureColor;
	//Clamps the specified value within the range of 0 to 1.
	color = saturate(color + specular);

	return color;
}