#include "modelData.h"

float GetShadow(float3 wpos,Texture2D<float4> tex, float2 offset)
{
	float shadow = 0.f;
	float depth = 0.f;
	float isShadow = 0.f;
	float isInUV = 0.f;
	[unroll(MAX_SHADOWMAP)]
	for(int i = 0; i < MAX_SHADOWMAP;i++)
	{
		float4 posinLVP = mul(mLVP[i], float4(wpos, 1));
		posinLVP.xyz /= posinLVP.w;

		float depthbuf = min(posinLVP.z / posinLVP.w, 1.0f);
		float2 smuv = float2(0.5f, -0.5f) * posinLVP.xy + float2(0.5f, 0.5f);

		float isInUVbuf = (1.0f-step(1.f,smuv.x)) * (1.0f-step(1.f,smuv.y)) * (1.0f-step(smuv.x,0.f)) * (1.0f-step(smuv.y,0.f));
		float2 pix = float2(ligPixSize[i].x * offset.x, ligPixSize[i].y * offset.y);

		float shadowbuf = 0.f;
		float is_inuvbuf_shadow_isuv = isInUVbuf * (1.0f - isShadow) * (1.0f-isInUV);
		shadowbuf += shadowMap_1.Sample(Sampler, smuv + pix * 0.5f).r * is_inuvbuf_shadow_isuv * (1.0f-abs(sign(0-i)));
		shadowbuf += shadowMap_2.Sample(Sampler, smuv + pix * 0.5f).r * is_inuvbuf_shadow_isuv * (1.0f-abs(sign(1-i)));
		shadowbuf += shadowMap_3.Sample(Sampler, smuv + pix * 0.5f).r * is_inuvbuf_shadow_isuv * (1.0f-abs(sign(2-i)));
		float isShadowbuf = (1.0f-step(depthbuf,shadowbuf + depthoffset[i]))*isInUVbuf*sign(shadowbuf);
		//depth += depthbuf * isShadowbuf;
		shadow += shadowbuf*(1.0f-isShadow)* isShadowbuf;
		isShadow = sign(isShadow+isShadowbuf);
		isInUV = sign(isInUV+isInUVbuf);
		/*
		if (smuv.x < 1.f && smuv.y < 1.f && smuv.x > 0.f && smuv.y > 0.f)
		{
			float2 pix = float2(ligPixSize[i].x * offset.x, ligPixSize[i].y * offset.y);
			if (i == 0)
			{
				shadow = shadowMap_1.Sample(Sampler, smuv + pix / 2).r;
			}
			else if (i == 1)
			{
				shadow = shadowMap_2.Sample(Sampler, smuv + pix / 2).r;
			}
			else if (i == 2)
			{
				shadow = shadowMap_3.Sample(Sampler, smuv + pix / 2).r;
			}
			int isShadow = !step(depth,shadow.r + depthoffset[i]);
			return float2(1.f*isShadow,(depth-shadow)*isShadow);
		}*/
	}
	//int isShadow = !step(depth,shadow.r + depthoffset[i]);
	return float2(1.f*isShadow,(depth-shadow)*isShadow);
	//return float2(0.f,0.f);
}


PSOutput PSProcess_GBuffer(float4 albe,PSInput In)
{
	PSOutput Out = (PSOutput)0;
	Out.diffuse = albe;
	Out.normal = float4(In.Normal,1.0f);
	Out.tangent = float4(In.Tangent,1.0f);
	Out.specular = float4(In.Pos,1.0f);			//スペキュラマップ
	Out.depth = In.PosInProj.z / In.PosInProj.w;

	float2 sdw = GetShadow(In.Pos, shadowMap_1,0);
	Out.shadow = float4(sdw,Out.depth.x*sdw.x,1);//x:影の有無 y:未使用 z:深度値(多分使わない) w:未使用.
	return Out;
}