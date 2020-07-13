#include "modelData.h"
#include "Math.h"

struct PSInputSSR {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

cbuffer SSRCBuffer:register(b7)
{
    //float4x4 projMat;
    //float4x4 invProjMat;
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
#define THICKNESS 32.f
#define BINARY_ITERATIONS 16.f
#define BYNARY_DECAY 0.5f

/*
float3 GetViewNormal(float2 uv)
{
    float3 normal = gNormalMap.Sample(Sampler,uv).xyz;
    return normalize(mul(viewMat,normal));
    //return 2.f * normal - 1.f;
}
*/

//ビュー空間でのZに変換。
float GetViewZ(float depth)
{
    return (camFar*camNear)/((camFar-camNear)*depth-camFar);
}

/*
float3 GetViewPos(float2 uv,float depth,float viewZ)
{
    float w = projMat[3][2] * viewZ + projMat[3][3];
    float4 clippos = float4((float3(uv,depth)-0.5f)*2.0f,1.0f);
    //float4 clippos = float4(uv*float2(2.0f,-2.0f)+float2(-1.0f,1.0f),depth,1.0f);
    clippos *= w;
    float3 res = mul(invProjMat,clippos).xyz;
    return res;
}*/

float4 PSMain_SSR(PSInputSSR In):SV_Target0
{
    float depth = gDepthMap.Sample(Sampler,In.uv).x;

    float3 worldNor = normalize(gNormalMap.Sample(Sampler,In.uv).xyz);  //法線。
    float3 worldPos = GetWorldPosition(In.uv,depth,invVPMat);           //ピクセルの位置。

    float3 c2p = worldPos-campos;   //ピクセルのポジションからカメラのポジションのベクトル
    float c2plen = length(c2p);     //その距離。
    clip((c2plen<=rayLen)-1.f); //描画範囲外なので切り捨て

    float3 cam2pos = normalize(c2p); //ピクセルのポジションからカメラのポジションの方向ベクトル
    float3 refvec = normalize(reflect(cam2pos,worldNor));   //反射

    float3 rayAdd = refvec*rayStep + refvec * pow(c2plen/rayLen,20)*4.f;    //1ステップ分のレイのベクトル

    float3 hitpos = worldPos;       //当たった位置。
    float isHit = 0.0f;             //ヒットした？
    float2 hitpix = float2(0,0);    //ヒットしたピクセルの位置。
    [unroll(RAY_ITERATIONS)]
    for(int i=0;i<RAY_ITERATIONS;i++)
    {
        hitpos += rayAdd + rayAdd * (c2plen/rayLen)*(RAY_ITERATIONS-i); //ステップ

        float4 wp = mul(VPMat,float4(hitpos,1.0f));     //カメラ行列変換。
        wp.xyz /= wp.w;
        hitpix = wp.xy * float2(0.5f, -0.5f) + 0.5f;    //UV空間に変換。
        float z = gDepthMap.Sample(Sampler,hitpix).x;   //当たったとこの深度値。
        float dist = GetViewZ(z)- GetViewZ(wp.z);       //レイの深度値と画面のピクセルの深度値の比較。

        //当たった？
        isHit = dist > 0.f && dist < THICKNESS /// (1+RAY_ITERATIONS-i)
                && hitpix.x <= 1.f && hitpix.y <= 1.f
                && hitpix.x >= 0.f && hitpix.y >= 0.f;

        [branch]
        if(isHit>0.f)   //当たった
            break;
    }
    clip(isHit-1.f);//ヒットしなかったので切り捨て
    
    //二分木探索。
    float isHitOld = 1.f;
    float3 slide = rayAdd*-BYNARY_DECAY;
    [unroll(BINARY_ITERATIONS)]
    for(int i=0;i < BINARY_ITERATIONS;i++)
    {
        float isHit;
        hitpos += slide;

        float4 wp = mul(VPMat,float4(hitpos,1.0f));
        wp.xyz /= wp.w;
        hitpix = wp.xy * float2(0.5f, -0.5f) + 0.5f;
        float z = gDepthMap.Sample(Sampler,hitpix).x;
        float dist = GetViewZ(z)- GetViewZ(wp.z);

        isHit = dist > 0.f && dist < THICKNESS /// (1+RAY_ITERATIONS-i)
                && hitpix.x <= 1.f && hitpix.y <= 1.f
                && hitpix.x >= 0.f && hitpix.y >= 0.f;

        //treuからfalseもしくはfalseからtrueになった場合折り返す。
        float check = abs(isHitOld - isHit);
        slide *= BYNARY_DECAY*(-1.f*check+(1.f-check));
    }
    

    float4 res = texture_1.Sample(Sampler,hitpix);
    res *= gSpecularMap.Sample(Sampler,In.uv).x;
    //res *= 0.5;//test用。
    return res;
}