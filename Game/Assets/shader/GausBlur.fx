
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
	float rttexRatio;
}

Texture2D<float4> srcTex:register(t0);	//�ڂ����e�N�X�`��
sampler Sampler:register(s0);

/// <summary>
/// 
/// </summary>
PS_BlurInput VSMain_X(VSInput In)
{
	float2 texSize;
	float level;
	srcTex.GetDimensions(0, texSize.x, texSize.y, level);
	PS_BlurInput Out;
	Out.pos = In.pos;
	float2 tex = In.uv;
	float pixSize = -1.0f / texSize.x;

	//tex += float2(pixSize*0.5f,0.f);
	/*
	Out.tex0 = tex + float2(-1.0f / texSize.x, 0.0f);
	Out.tex1 = tex + float2(-3.0f / texSize.x, 0.0f);
	Out.tex2 = tex + float2(-5.0f / texSize.x, 0.0f);
	Out.tex3 = tex + float2(-7.0f / texSize.x, 0.0f);
	Out.tex4 = tex + float2(-9.0f / texSize.x, 0.0f);
	Out.tex5 = tex + float2(-11.0f / texSize.x, 0.0f);
	Out.tex6 = tex + float2(-13.0f / texSize.x, 0.0f);
	Out.tex7 = tex + float2(-15.0f / texSize.x, 0.0f);
	*/

	float addpix = pixSize*rttexRatio;
	Out.tex0 = tex + float2(pixSize, 0.0f);
	Out.tex1 = tex + float2(pixSize+addpix*1.f, 0.0f);
	Out.tex2 = tex + float2(pixSize+addpix*2.f, 0.0f);
	Out.tex3 = tex + float2(pixSize+addpix*3.f, 0.0f);
	Out.tex4 = tex + float2(pixSize+addpix*4.f, 0.0f);
	Out.tex5 = tex + float2(pixSize+addpix*5.f, 0.0f);
	Out.tex6 = tex + float2(pixSize+addpix*6.f, 0.0f);
	Out.tex7 = tex + float2(pixSize+addpix*7.f, 0.0f);
	return Out;
}

/// <summary>
/// 
/// </summary>
PS_BlurInput VSMain_Y(VSInput In)
{
	float2 texSize;
	float level;
	srcTex.GetDimensions(0, texSize.x, texSize.y, level);
	PS_BlurInput Out;
	Out.pos = In.pos;
	float2 tex = In.uv;

	float pixSize = -1.0f / texSize.y;
	//tex += float2(0, pixSize*0.5f);
	/*
	Out.tex0 = tex + float2(0.0f, -1.0f / texSize.y);
	Out.tex1 = tex + float2(0.0f, -3.0f / texSize.y);
	Out.tex2 = tex + float2(0.0f, -5.0f / texSize.y);
	Out.tex3 = tex + float2(0.0f, -7.0f / texSize.y);
	Out.tex4 = tex + float2(0.0f, -9.0f / texSize.y);
	Out.tex5 = tex + float2(0.0f, -11.0f / texSize.y);
	Out.tex6 = tex + float2(0.0f, -13.0f / texSize.y);
	Out.tex7 = tex + float2(0.0f, -15.0f / texSize.y);
	*/
	/*
	Out.tex0 = tex + float2(0.0f, pixSize*1.0f);
	Out.tex1 = tex + float2(0.0f, pixSize*2.0f);
	Out.tex2 = tex + float2(0.0f, pixSize*3.0f);
	Out.tex3 = tex + float2(0.0f, pixSize*4.0f);
	Out.tex4 = tex + float2(0.0f, pixSize*5.0f);
	Out.tex5 = tex + float2(0.0f, pixSize*6.0f);
	Out.tex6 = tex + float2(0.0f, pixSize*7.0f);
	Out.tex7 = tex + float2(0.0f, pixSize*8.0f);
	*/
	float addpix = pixSize*rttexRatio;
	Out.tex0 = tex + float2(0.0f,pixSize);
	Out.tex1 = tex + float2(0.0f,pixSize+addpix*1.f);
	Out.tex2 = tex + float2(0.0f,pixSize+addpix*2.f);
	Out.tex3 = tex + float2(0.0f,pixSize+addpix*3.f);
	Out.tex4 = tex + float2(0.0f,pixSize+addpix*4.f);
	Out.tex5 = tex + float2(0.0f,pixSize+addpix*5.f);
	Out.tex6 = tex + float2(0.0f,pixSize+addpix*6.f);
	Out.tex7 = tex + float2(0.0f,pixSize+addpix*7.f);
	return Out;
}

/// <summary>
/// 
/// </summary>
float4 PSMain(PS_BlurInput In):SV_Target0
{
	float4 Color;
	Color  = weight[0].x * (srcTex.Sample(Sampler, In.tex0)
						+ srcTex.Sample(Sampler, In.tex7 + offset.xy));
	Color += weight[0].y * (srcTex.Sample(Sampler, In.tex1)
						+ srcTex.Sample(Sampler, In.tex6 + offset.xy));
	Color += weight[0].z * (srcTex.Sample(Sampler, In.tex2)
						+ srcTex.Sample(Sampler, In.tex5 + offset.xy));
	Color += weight[0].w * (srcTex.Sample(Sampler, In.tex3)
						+ srcTex.Sample(Sampler, In.tex4 + offset.xy));
	Color += weight[1].x * (srcTex.Sample(Sampler, In.tex4)
						+ srcTex.Sample(Sampler, In.tex3 + offset.xy));
	Color += weight[1].y * (srcTex.Sample(Sampler, In.tex5)
						+ srcTex.Sample(Sampler, In.tex2 + offset.xy));
	Color += weight[1].z * (srcTex.Sample(Sampler, In.tex6)
						+ srcTex.Sample(Sampler, In.tex1 + offset.xy));
	Color += weight[1].w * (srcTex.Sample(Sampler, In.tex7)
						+ srcTex.Sample(Sampler, In.tex0 + offset.xy));
	//return float4(0,0,1,1);
	return Color;
	//return float4(Color.xyz,1);
}