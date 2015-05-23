

Texture2D shaderTexture : register(t0);
Texture2D projectionTexture : register(t1);

SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
    float padding;
};


struct VS_OUT
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
    float4 ViewPos : TEXCOORD1;
};



float4 PS(VS_OUT input) : SV_TARGET
{
    float4 color;
    float3 lightDir;
    float lightIntensity;
    float4 textureColor;
    float2 projectTexCoord;
    float4 projectionColor;


    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

    // Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.Normal, lightDir));

    if(lightIntensity > 0.0f)
    {
        // Determine the light color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
    }

    // Saturate the light color.
    color = saturate(color);

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.Tex);

    // Combine the light color and the texture color.
    color = color * textureColor;

    // Calculate the projected texture coordinates.
    projectTexCoord.x =  input.ViewPos.x / input.ViewPos.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.ViewPos.y / input.ViewPos.w / 2.0f + 0.5f;

	    // Determine if the projected coordinates are in the 0 to 1 range.  If it is then this pixel is inside the projected view port.
    if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        // Sample the color value from the projection texture using the sampler at the projected texture coordinate location.
        projectionColor = projectionTexture.Sample(SampleType, projectTexCoord);

        // Set the output color of this pixel to the projection texture overriding the regular color value.
        color = projectionColor;
    }

    return color;
}