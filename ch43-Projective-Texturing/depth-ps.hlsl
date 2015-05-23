
struct VS_OUT
{
	 float4 Position  :SV_POSITION;
	  float4 depthPosition : TEXTURE0;
};

float4 PS(VS_OUT vs_out): SV_TARGET
{
    float depthValue;
	float4 color;
	
	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	depthValue = vs_out.depthPosition.z / vs_out.depthPosition.w;
	color = float4(depthValue, depthValue, depthValue, 1.0f);
	return color;
}