
struct VS_OUT
{
	 float4 Position  :POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 normal    :NORMAL;
	 float3 tangent   :TANGENT;
	 float3 binormal  :BINORMAL;
};

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
};

Texture2D shaderTextures[2];
SamplerState SampleType;

float4 PS(VS_OUT vs_out): SV_TARGET
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float3 lightDir;
    float lightIntensity;
    float4 color;

    textureColor = shaderTextures[0].Sample(SampleType, vs_out.Tex);
    bumpMap = shaderTextures[1].Sample(SampleType, vs_out.Tex);

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;

    // Calculate the normal from the data in the bump map.
    bumpNormal = (bumpMap.x * vs_out.tangent) + (bumpMap.y * vs_out.binormal) + (bumpMap.z * vs_out.normal);
	
    // Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);

    // Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel based on the bump map normal value.
    lightIntensity = saturate(dot(bumpNormal, lightDir));

    // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
    color = saturate(diffuseColor * lightIntensity);

    // Combine the final bump light color with the texture color.
    color = color * textureColor;
	return color;
}