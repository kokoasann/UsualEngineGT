#include "Math.h"

cbuffer CBuffer:register(b7)
{
    float3 effectPos;
    float2 effectSize;

    float attenuationY;			//Y減衰。
    float attenuationXZ;	    //XZ減衰。
    float concentration;		//濃さ。

    float2 screenOffset;
    float2 screenSize;
    float4 mVPI;
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

    
}