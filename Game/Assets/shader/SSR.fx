#include "modelData.h"

struct PSInputSSR {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

cbuffer SSRCBuffer:register(b7)
{
    float4x4 projMat;
    float4x4 invProjMat;
    float camFar;
    float camNear;
    float rayLen;
    float rayStep;

}
#define RAY_ITERATIONS 8
#define THICKNESS 16
#define BINARY_ITERATIONS 8

float3 GetViewNormal(float2 uv)
{
    float3 normal = gNormalMap.Sample(Sampler,uv).xyz;
    return normal*2.f-1.f;
}

float GetViewZ(float depth)
{
    return (camFar*camNear)/((camFar-camNear)*depth-camFar);
}

float3 GetViewPos(float2 uv,float depth,float viewZ)
{
    float w = projMat[3][2] * viewZ + projMat[3][3];
    float4 clippos = float4((float3(uv,depth)-0.5f)*2.0f,1.0f);
    clippos *= w;
    float3 res = mul(invProjMat,clippos).xyz;
    return res;
}

float4 PSMain_SSR(PSInputSSR In)
{
    float depth = gDepthMap.Sample(Sampler,In.uv).x;

    float viewZ = GetViewZ(depth);
    float3 viewPos = GetViewPos(In.uv,depth,viewZ);
    float3 viewNor = GetViewNormal(In.uv);

    float3 cam2pos = normalize(viewPos);
    float3 refvec = reflect(cam2pos,viewNor);

    float3 rayAdd = refvec*rayStep;

    float3 hitpos = viewPos;
    float isHit = 0.0f;
    float2 hitpix = float2(0);
    [unroll(RAY_ITERATIONS)]
    for(int i=0;i<RAY_ITERATIONS;i++)
    {
        hitpos += rayAdd;
        float4 cl = mul(projMat,float4(hitpos,1.0f));
        float2 p = cl.xy/cl.w;
        hitpix = (p*0.5f+0.5f) + hitpix;

        float viewZ = GetViewZ(gDepthMap.Sample(Sampler,In.uv).x);
        float dist = hitpos.z - viewZ;
        isHit = dist < 0.f && dist > -THICKNESS;

        [branch]
        if(isHit>0.f)
            break;
    }

    /*
    [branch]
    if(isHit > 0.f)
    {
        float3 slide = rayAdd*-0.5f;
        [unroll(BINARY_ITERATIONS)]
        for(int i=0;i < BINARY_ITERATIONS;i++)
        {

        }
    }
    */
    

    return float4(0);
}