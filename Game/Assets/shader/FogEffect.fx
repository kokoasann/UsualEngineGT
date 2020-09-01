#include "Math.h"
#include "PerlinNoise.fxh"
#include "WorleyNoise.fxh"

cbuffer CBuffer:register(b7)
{
    float4 effectCol;
    float3 effectPos;
    float effectRadius  :packoffset(c1.w);
    float3 effectTip    :packoffset(c2.x);

    float attenuationY  :packoffset(c2.w);			//Y減衰。
    float attenuationXZ :packoffset(c3.x);	    //XZ減衰。

    float pConcentration;   //パーリンノイズの濃度。
    float pDisperse;        //パーリンノイズの散乱具合。
    float pScale;           //パーリンノイズのスケール。
    float wConcentration;   //ウォーリーノイズの濃度。
    float wDisperse;        //ウォーリーノイズの散乱具合。
    float wScale;           //ウォーリーノイズのスケール。

    float3 mainLightDir :packoffset(c4.w);

    float2 screenOffset :packoffset(c5.z);
    float2 screenSize   :packoffset(c6.x);
    float4x4 mProjI     :packoffset(c6.z);
};
sampler Sampler : register(s0);
Texture2D<float4> gDepthMap : register(t7);

struct PSInput
{
    float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
}

#define RAY_COUNT 10.f


float4 PSMain_FogEffect(PSInput In):VS_Target0
{
    float2 screenPos = mad(In.uv,screenSize,screenOffset)/float2(1280.f,720.f);
    float wdepth = gDepthMap.Sample(Sampler,screenPos).x;

    float3 viewpos = mul(mProjI,float4(screenPos*float2(2.0f,-2.0f)+float2(-1.0f,1.0f),effectTip.z,1.f)).xyz;
    float3 rayDir = normalize(viewpos);
    
    float noiseRate = 0.5f;

    float cloud = 0.f;
    float foundation = 0.f;
    int rayCount = 10;
    float rayStep = effectRadius * 0.1f;
    [unroll(rayCount)]
    for(int i = 0; i < rayCount; i++)
    {
        float3 raypos = viewpos+rayDir*(rayStep*i);
        [branch]
        if(length(raypos-effectPos)>effectRadius)
        {
            continue;
        }
        float blendNoise;
        float hrate = 1.f;//min(cloudHeight / max(rayStep*i,0.0000001f),1.f);
        {
            float pernoise = mad(PerlinNoise3D(raypos*pScale),pConcentration,-pDisperse);
            float wornoise = mad(1.f-WorleyNoise3D(raypos*wScale),wConcentration,-wDisperse);

            //float noise = saturate(pernoise*noiseRate + wornoise*(1.f-noiseRate));
            float noise = saturate(mad(noiseRate,(pernoise-noiseRate),wornoise));

            //float blend = (1.f-PerlinNoise3D(raypos*distribution))*concentration-disperse;
            blendNoise = noise*hrate;
            cloud += blendNoise;
        }
        cloud = saturate(cloud);

        //ライティング
        //raypos += mainLightDir*-rayStep;
        raypos = mad(mainLightDir,-rayStep,raypos);
        {
            float ligHrate = 1.f;//min(cloudHeight / max(rayStep*i,0.0000001f),1.0f);

            float pernoise = mad(PerlinNoise3D(raypos*pScale),pConcentration,-pDisperse);
            float wornoise = mad(1.f-WorleyNoise3D(raypos*wScale),wConcentration,-wDisperse);

            float noise = saturate(mad(noiseRate,(pernoise-noiseRate),wornoise));

            //float blend = (1.f-PerlinNoise3D(raypos*distribution))*concentration-disperse;
            foundation += noise*ligHrate*blendNoise*(1.f-cloud);
        }
    }
    foundation = saturate(foundation);
    float3 color = lerp(float3(0.95f,0.95f,0.95f), float3(0.45f, 0.45f, 0.8f), foundation);
    color *= effectCol.xyz;
    return float4(color,cloud);
}