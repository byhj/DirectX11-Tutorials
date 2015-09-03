cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};


cbuffer ScreenSizeBuffer
{
	float screenWidth = 500.0;
	float3 padding;
};


struct VS_IN
{
	 float4 Pos       :POSITION;
	 float2 Tex       :TEXCOORD0;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};

VS_OUT VS(VS_IN vs_in)
{
    VS_OUT vs_out;

	float texelSize;
    vs_in.Pos.w = 1.0f;

    vs_out.Pos = mul(vs_in.Pos, world);
    vs_out.Pos = mul(vs_out.Pos, view);
    vs_out.Pos = mul(vs_out.Pos, proj);
    
    vs_out.Tex = vs_in.Tex;

	// Determine the floating point size of a texel for a screen with this specific height.
	texelSize = 1.0f / screenWidth;

	// Create UV coordinates for the pixel and its four vertical neighbors on either side.
	vs_out.texCoord1 = vs_in.Tex + float2(0.0f, texelSize * -4.0f);
	vs_out.texCoord2 = vs_in.Tex + float2(0.0f, texelSize * -3.0f);
	vs_out.texCoord3 = vs_in.Tex + float2(0.0f, texelSize * -2.0f);
	vs_out.texCoord4 = vs_in.Tex + float2(0.0f, texelSize * -1.0f);
	vs_out.texCoord5 = vs_in.Tex + float2(0.0f, texelSize *  0.0f);
	vs_out.texCoord6 = vs_in.Tex + float2(0.0f, texelSize *  1.0f);
	vs_out.texCoord7 = vs_in.Tex + float2(0.0f, texelSize *  2.0f);
	vs_out.texCoord8 = vs_in.Tex + float2(0.0f, texelSize *  3.0f);
	vs_out.texCoord9 = vs_in.Tex + float2(0.0f, texelSize *  4.0f);

    return vs_out;
}