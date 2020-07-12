#include "modelData.h"
#include "Math.h"

struct PSInputSSR {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

cbuffer SSRCBuffer:register(b7)
{
    float4x4 projMat;
    float4x4 invProjMat;
    float4x4 viewMat;
    float4x4 VPMat;
    float4x4 invVPMat;
    float3 campos;
    float camFar;
    float camNear;
    float rayLen;
    float rayStep;
}
#define RAY_ITERATIONS 64.f
#define THICKNESS 0.01f
#define BINARY_ITERATIONS 8.f

float3 GetViewNormal(float2 uv)
{
    float3 normal = gNormalMap.Sample(Sampler,uv).xyz;
    return normalize(mul(viewMat,normal));
    //return 2.f * normal - 1.f;
}

float GetViewZ(float depth)
{
    return (camFar*camNear)/((camFar-camNear)*depth-camFar);
}

float3 GetViewPos(float2 uv,float depth,float viewZ)
{
    float w = projMat[3][2] * viewZ + projMat[3][3];
    float4 clippos = float4((float3(uv,depth)-0.5f)*2.0f,1.0f);
    //float4 clippos = float4(uv*float2(2.0f,-2.0f)+float2(-1.0f,1.0f),depth,1.0f);
    clippos *= w;
    float3 res = mul(invProjMat,clippos).xyz;
    return res;
}

float4 PSMain_SSR(PSInputSSR In):SV_Target0
{
    float depth = gDepthMap.Sample(Sampler,In.uv).x;

    float viewZ = GetViewZ(depth);
    float3 viewPos = GetViewPos(In.uv,depth,viewZ);
    float3 viewNor = GetViewNormal(In.uv);

    float3 worldNor = normalize(gNormalMap.Sample(Sampler,In.uv).xyz);
    float3 worldPos = GetWorldPosition(In.uv,depth,invVPMat);
    float3 cam2pos = normalize(worldPos-campos);
    float3 refvec = normalize(reflect(cam2pos,worldNor));

    //float3 cam2pos = normalize(viewPos);
    //float3 refvec = normalize(reflect(cam2pos,viewNor));

    float3 rayAdd = refvec*rayStep;

    float3 hitpos = worldPos;
    //float3 hitpos = viewPos;
    float isHit = 0.0f;
    float2 hitpix = float2(0,0);
    //[unroll(RAY_ITERATIONS)]
    for(int i=0;i<RAY_ITERATIONS;i++)
    {
        hitpos += rayAdd;

        float4 cl = mul(projMat,float4(hitpos,1.0f));
        float2 p = cl.xy/cl.w;
        hitpix = (p*0.5f+0.5f);//*(1.f-isHit) + hitpix*isHit;

        float viewZ = GetViewZ(gDepthMap.Sample(Sampler,hitpix).x);
        //float dist = hitpos.z - viewZ;

        float4 wp = mul(VPMat,float4(hitpos,1.0f));
        wp.xyz /= wp.w;
        wp.xy = wp.xy * float2(0.5f, -0.5f) + 0.5f;
        hitpix = wp.xy;
        float z = gDepthMap.Sample(Sampler,hitpix).x;
        float dist = wp.z - z;

        isHit = dist > 0.f && dist < THICKNESS / (1+RAY_ITERATIONS-i)
                && hitpix.x <= 1.f && hitpix.y <= 1.f
                && hitpix.x >= 0.f && hitpix.y >= 0.f;

        [branch]
        if(isHit>0.f)
            break;
    }

    float4 res = texture_1.Sample(Sampler,hitpix) * isHit;

    
    [branch]
    if(isHit > 0.f)
    {
        float3 slide = rayAdd*-0.5f;
        [unroll(BINARY_ITERATIONS)]
        for(int i=0;i < BINARY_ITERATIONS;i++)
        {

        }
    }
    
    
    res *= gSpecularMap.Sample(Sampler,In.uv).x;

    return res;
}