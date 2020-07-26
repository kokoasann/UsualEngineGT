#include "modelData.h"
#include "PerlinNoise.fxh"
#include "Math.h"

float GetShadow(float3 wpos, float2 offset)
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
		float isShadowbuf = (1.0f-step(depthbuf,shadowbuf + depthoffset[i]*2.f))*isInUVbuf*sign(shadowbuf);
		//depth += depthbuf * isShadowbuf;
		shadow += shadowbuf*(1.0f-isShadow)* isShadowbuf;
		isShadow = sign(isShadow+isShadowbuf);
		isInUV = sign(isInUV+isInUVbuf);
	}
	return isShadow;
}


cbuffer CBuffer:register(b7)
{
    float4x4 mVP;
    float4x4 mVPI;
    float3 mainLightDir;
    float3 camPos;
    float camFar;
    float camNear;
    float offset;
}

//ビュー空間でのZに変換。
float GetViewZ(float depth)
{
    return (camFar*camNear)/((camFar-camNear)*depth-camFar);
}


struct PSInput_RMFog
{
    float4 pos:SV_Position;
    float2 uv:TEXCOORD0;
};

float4 PSMain_RMFog(PSInput_RMFog In):SV_Target0
{
    float gdepth = gDepthMap.Sample(Sampler,In.uv).x;

    float fogHeight = 200.f;

    float concentration = 0.5f;
    float disperse = -0.4f;

    #define rayCount 10.f
    float rayStep = 400.f;
    
    float radius = 5.f;

    float fogScale = 0.008f;
    float blendScale = 0.002;

    
    float3 startPos = GetWorldPosition(In.uv,0.f,mVPI);
    float3 endPos = GetWorldPosition(In.uv,0.5f,mVPI);
    float3 s2e = endPos-startPos;
    float3 rayDir = normalize(s2e);


    float foundation = 0.f;
    float fog = 0.f;
    float3 col = float3(1.f,1.f,1.f);

    {
        float3 rayPos = startPos+rayDir*offset*100.f;

        

        float pernoise = 1.f-PerlinNoise3D(rayPos*fogScale);
        float blend = (1.f-PerlinNoise3D(rayPos*blendScale))*concentration+disperse;

        float hrate = fogHeight/(abs(rayPos.y)+0.1f);
        float f = pernoise * blend;
        f *= min(hrate,1.f);
        float shadow = (GetShadow(rayPos,0.f)-0.5f)*2.f*blend;

        {
            rayPos += mainLightDir*200.f;
            float blend = (1.f-PerlinNoise3D(rayPos*blendScale))*concentration+disperse;
            col -= lerp(float3(0,0,0),(float3(1.f,1.f,1.f)-float3(0.45f, 0.4f, 0.6f))*0.2f,blend);
        }

        foundation += shadow;
        fog += f;
    }
    
    [unroll(rayCount)]
    for(int i=1;i<=rayCount;i++)
    {
        float3 rayPos = startPos+rayDir*rayStep*(float)i;

        {
            float4 vpp =  mul(mVP,float4(rayPos,1.f));
            float dep = vpp.z / vpp.w;
            [branch]
            if(dep>=gdepth+0.001f)
                break;
        }
        rayPos += rayDir*offset*1000.f;

        float pernoise = 1.f-PerlinNoise3D(rayPos*fogScale);
        float blend = (1.f-PerlinNoise3D(rayPos*blendScale))*concentration+disperse;

        float hrate = fogHeight/(abs(rayPos.y)+0.1f);
        float f = pernoise * blend;
        f *= min(hrate,1.f);
        float shadow = (GetShadow(rayPos,0.f)-0.5f)*2.f*blend;

        {
            rayPos += mainLightDir*200.f;
            float blend = (1.f-PerlinNoise3D(rayPos*blendScale))*concentration+disperse;
            col -= lerp(float3(0,0,0),(float3(1.f,1.f,1.f)-float3(0.45f, 0.4f, 0.6f))*0.2f,blend);
        }

        foundation += shadow;
        fog += f;
    }
    fog = clamp(fog,0.f,0.95f);
    foundation = clamp(foundation,0.f,1.f);

    //col *= lerp(float3(1,1,1),float3(0.45f, 0.4f, 0.6f),foundation);

    return float4(col,fog);
}