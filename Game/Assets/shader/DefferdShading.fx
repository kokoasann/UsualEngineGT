#include "modelData.h"


cbuffer DefferdCB:register(b0)
{
    float4 camDir;
}



struct DefferdVSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};
struct DefferdPSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

DefferdPSInput VSMain_Defferd(DefferdVSInput In)
{
    DefferdPSInput Out;
    Out.pos = In.pos;
    Out.uv = In.uv;
    return Out;
}

float4 PSMain_Defferd(DefferdPSInput In):SV_TARGET0
{
    float4 diffuse = gDiffuseMap.Sample(Sampler,In.uv);
    float3 normal = gNormalMap.Sample(Sampler,In.uv);
    float4 col = diffuse;
    float depth = gDepthMap.Sample(Sampler,In.uv);
    
    float3 li = 0.f;
    float dotshadow = 0.f;
	for (int i = 0; i < DLcount; i++)
	{
        float rad = dot(DirLights[i].dir * -1.f, normal);
        float threshold = 0.174533f;                        //いずれ未来の自分が改善してくれているはず!
		float k = step(threshold,rad);
        

		//li = float3(0.45f, 0.4f, 0.6f) * !k;//影が付く!
        //li += float3(1.f,1.f,1.f)*k;
        dotshadow = 1.0f-k;
    }
    
    float3 pos = gSpecularMap.Sample(Sampler,In.uv);

    #if 0
    float3 v = pos-eyepos;
    v=normalize(v);
	float r = dot(v,normal);
	pos += normal*r*2.f;
    v = pos-eyepos;
    v=normalize(v);
	float sp = max(0,dot(DirLights[0].dir,v));
    #endif

    float3 R = -camDir.xyz + 2 * dot(camDir.xyz, normal) * normal;
    float rad = dot(DirLights[0].dir, R);
    float sp = max(0.f, rad);
    float4 spGradation = texture_1.Sample(Sampler,float2(lerp(1.f,0.f,sp),1.f));  //グラデーションマップ(明かり用)
    

    float4 sha = gShadowMap.Sample(Sampler,In.uv);
    
    
    /*
    float r = min(10,sha2.y);
    r/=10;
    float s =lerp(sha,sha2,r);*/
    //s = lerp(0,1,sha);
    sha.x = min(sha.x+dotshadow,1.f);
    //sha.x = texture_1.Sample(Sampler,float2(lerp(1.f,0.f,sha.x),0.f)).x;  //グラデーションマップ(影用)
    li = lerp(float3(0.f,0.f,0.f),float3(0.45f, 0.4f, 0.6f),sha.x);
    //li.xyz *= step(sha.z,depth);
    //col.xyz *= li;

    li += (DirLights[0].color.xyz + DirLights[0].color.xyz * spGradation.x)*(1.f-sha.x);
    col.xyz *= li;

    
    float4 usu = lerp(float4(0.8f, 0.88f, 1.f,1.f),float4(0.f,0.f,0.f,0.f),min(lerp(1,0,pow(depth,6000)),1));
    col.xyz *= 1.f-usu.w;
    col.xyz += usu;
    
    //return float4(sha.x, sha.x, sha.x,1);
    return col;
}


/*
        以下、デバッグ用。
*/
float4 PSMain_Diffuse(DefferdPSInput In) :SV_TARGET0
{
    return gDiffuseMap.Sample(Sampler,In.uv);
}
float4 PSMain_Normal(DefferdPSInput In) : SV_TARGET0
{
    return gNormalMap.Sample(Sampler,In.uv);
}
float4 PSMain_Depth(DefferdPSInput In) : SV_TARGET0
{
    float dp = gDepthMap.Sample(Sampler,In.uv);
    dp = pow(dp, 2000);
    return float4(dp, dp, dp, 1.0f);
}
float4 PSMain_Shadow(DefferdPSInput In) :SV_TARGET0
{
    float s = gShadowMap.Sample(Sampler,In.uv);
    return float4(s, s, s, 1.0f);
}