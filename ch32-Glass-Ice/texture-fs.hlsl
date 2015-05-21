
struct VS_OUT
{
	 float4 Position  :SV_POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 normal    :NORMAL;
	 float3 tangent   :TANGENT;
	 float3 binormal  :BINORMAL;
	 float3 viewDirection :TEXCOORD1;
};

cbuffer LightBuffer
{
	float4 diffuseColor;
	float4 specularColor;
	float specularPower;
	float3 lightDirection;
};

Texture2D shaderTextures[3];
SamplerState SampleType;

float4 PS(VS_OUT vs_out): SV_TARGET
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float3 lightDir;
    float  lightIntensity;
    float4 color;
    float4 specularIntensity;
    float3 reflection;
    float4 specular;

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

	if(lightIntensity > 0.0f)
    {        // Sample the pixel from the specular map texture.
        specularIntensity = shaderTextures[2].Sample(SampleType, vs_out.Tex);
	        // Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * bumpNormal - lightDir); 

        // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate( dot(reflection, vs_out.viewDirection) ), specularPower);
		        // Use the specular map to determine the intensity of specular light at this pixel.
        specular = specular * specularIntensity;
		
        // Add the specular component last to the output color.
        color = saturate(color + specular);
    }
		
	return color;
}