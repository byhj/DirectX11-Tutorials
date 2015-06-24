
struct VS_IN
{
	 float4 Pos       :POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 Normal    :NORMAL;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

VS_OUT VS(VS_IN vs_in)
{	
 
   VS_OUT vs_out;
   vs_out.Pos = vs_in.Pos;
   vs_out.Tex = vs_in.Tex;

   return vs_out;
}
