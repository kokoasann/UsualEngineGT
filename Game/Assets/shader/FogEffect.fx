#include "Math.h"
#include "PerlinNoise.fxh"
#include "WorleyNoise.fxh"

cbuffer CBuffer:register(b7)
{
    float4 effectCol;
    float3 effectPos;
    float effectRadius  :packoffset(c1.w);
    float3 effectTip;

    float attenuationY;			//Y減衰。
    float attenuationXZ;	    //XZ減衰。
    float concentration;		//濃さ。
    float pScale;
    float wScale;

    float2 screenOffset;
    float2 screenSize;
    float4 mVPI;
    float4x4 mProjI
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
    
    int rayCount = 10;
    float rayStep = effectRadius * 0.1f;
    [unroll(rayCount)]
    for(int i = 0; i < rayCount; i++)
    {
        float3 raypos = viewpos+rayDir*rayStep*i;
        float blendNoise;
        float3 sampPos = raypos;
        float hrate = 1.f;//min(cloudHeight / max(rayStep*i,0.0000001f),1.f);
        {
            float pernoise = PerlinNoise3D(sampPos*pScale);
            float wornoise = 1.f-WorleyNoise3D(sampPos*wScale);

            float noise = clamp(pernoise*noiseRate + wornoise*(1.f-noiseRate),0.f,1.f);

            float blend = (1.f-PerlinNoise3D(raypos*distribution))*concentration+disperse;
            //float wblend = 1.f-WorleyNoise3D(sampPos*0.0003f)*concentration+disperse;
            //blend = blend*0.5f+wblend*0.5f;
            blendNoise = noise*blend*hrate;
            cloud += blendNoise;
        }
        cloud = clamp(cloud,0.f,1.f);

        //ライティング
        raypos += mainLightDir*-100.f;
        {
            float ligHrate = min(cloudHeight / max(rayStep*i,0.0000001f),1.0f);

            float pernoise = PerlinNoise3D(raypos*pScale);
            float wornoise = 1.f-WorleyNoise3D(raypos*wScale);

            float noise = clamp(pernoise*noiseRate + wornoise*(1.f-noiseRate),0.f,1.f);

            float blend = (1.f-PerlinNoise3D(raypos*distribution))*concentration+disperse;
            //float wblend = 1.f-WorleyNoise3D(raypos*0.0003f)*concentration+disperse;
            //blend = blend*0.5f+wblend*0.5f;
            //color -= lerp(float3(0,0,0),(float3(1.f,1.f,1.f)-float3(0.45f, 0.4f, 0.6f))*2.5f,noise*blend*ligHrate)*blendNoise*(1.f-cloud);
            foundation += noise*blend*ligHrate*blendNoise*(1.f-cloud);
        }
        
    }
}