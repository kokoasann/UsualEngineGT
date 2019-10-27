
struct VSInput
{
	float4 pos:SV_POSITION;
	float2 uv:TEXCOORD0;
};
struct PS_BlurInput 
{
	float4 pos	: SV_POSITION;
	float2 tex0	: TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
	float2 tex4 : TEXCOORD4;
	float2 tex5 : TEXCOORD5;
	float2 tex6 : TEXCOORD6;
	float2 tex7 : TEXCOORD7;
};

cbuffer BlurParam:register(b0)
{
	float4 offset;
	float4 weight[2];
}

Texture2D<float4> srcTex:register(t0);	//ぼかすテクスチャ
sampler Sampler:register(s0);

/// <summary>
/// 
/// </summary>
PS_BlurInput VSMain_X(VSInput In)
{

}
/// <summary>
/// 
/// </summary>
PS_BlurInput VSMain_Y(VSInput In)
{

}

/// <summary>
/// 
/// </summary>
float4 PSMain(PS_BlurInput In)
{

}