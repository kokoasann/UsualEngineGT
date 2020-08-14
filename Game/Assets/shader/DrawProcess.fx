#include "modelData.h"
#include "lightData.h"
#include "Math.h"

cbuffer DefferdCB:register(b7)
{
    float4x4 mViewProjInv;
    float3 camDirm;

}

#define threshold 0.5f


float4 DirectionLightProcess(float3 normal,float specular,float shadowOrg,float depthShadow,float3 R)
{
    float3 foundation = float3(0,0,0);
    float shadow = shadowOrg;
    for (int i = 1; i < DLcount; i++)
    {
        float rad = dot(DirLights[i].dir * -1.f, normal);
        //float threshold = 0.174533f;                        //いずれ未来の自分が改善してくれているはず!
        float k = step(threshold,rad);
        
        float sha = (1.0f-k)*(1.0f-shadow);
        float3 lig = DirLights[i].color.xyz*k;

        float sp = saturate(dot(DirLights[i].dir, R));
        sp *= specular;
        float4 spGradation = texture_1.Sample(Sampler,float2(saturate(sp),0.f));  //グラデーションマップ(明かり用)

        foundation += mad(spGradation.x,lig,lig)*(1.0f-sha);
        shadow += -1.0f * (shadow * (1.0f-sha));
    }
    return float4(foundation,shadow);
}


float3 PointLightProcess(float3 worldPos,float3 normal,float specular)
{
    float3 foundation = float3(0,0,0);
    for(int i=0;i<PLcount;i++)
    {
        float3 lendir = worldPos - PntLights[i].pos;
        float lendirP2 = dot(lendir,lendir);
        float3 dir = lendir * rsqrt(lendirP2);

        float rad = dot(dir * -1.f, normal);
        //float threshold = 0.174533f;                        //いずれ未来の自分が改善してくれているはず!
		float k = step(threshold,rad);
        
        float len = sqrt(lendirP2);

        k = min(k, step(len,PntLights[i].radius));

        /*
        float ligPow = max(0.0f,rad) / (1.0f + len);    // V / 1+len
        ligPow *= 10.f;
        ligPow = max(0,min(1.f,ligPow));
        ligPow *= specular;
        */

        float ligPow = (len*PntLights[i].decay);//PntLights[i].radius;
        //ligPow *= specular;
        float ligDecay = texture_1.Sample(Sampler,float2(saturate(ligPow),0.f)).x;
        {
            ligDecay *= floor(ligPow);
        }
        //float3 decayColor = PntLights[i].color.xyz * ligDecay;

        float colorPow = length(PntLights[i].color.xyz);
        float spGradation = texture_1.Sample(Sampler,float2(saturate(rad-0.001f),1.f)).x;  //グラデーションマップ(明かり用)
        //float3 ligcolor = PntLights[i].color.xyz/len;
        //float3 ligcolor = PntLights[i].color.xyz*max(colorPow-len,0.f);
        //float3 lig = (ligcolor*(spGradation.x));
        float spe = saturate(spGradation);
        float3 lig = (PntLights[i].color.xyz*max(spe-ligDecay,0.f));
        //lig = float3(max(lig.x,0.f),max(lig.y,0.f),max(lig.z,0.f));
        foundation += lig*k;

        //反射
        float3 w2p = worldPos - PntLights[i].pos;
        float3 e2w = worldPos - eyepos;
        w2p = normalize(w2p);
        e2w = normalize(e2w);
        float3 w2pRef =  -w2p + 2.0f * dot(w2p,normal) * normal;
        float refDote2w = dot(w2pRef,e2w);

        float sp = pow(saturate(refDote2w),5);

        spGradation = texture_1.Sample(Sampler,float2(saturate(sp),0.f)).x;  //グラデーションマップ(明かり用)
        //sp *= spGradation.x;
        lig = PntLights[i].color.xyz*(saturate((spGradation-ligDecay)*spe));
        //lig = float3(max(lig.x,0.f),max(lig.y,0.f),max(lig.z,0.f));
        foundation += lig*(specular*k);
    }
    return foundation;
}

float4 DrawProcess(float4 diffuse,float3 normal,float specular,float gshadow,float depth,float2 uv)
{
    float4 col = diffuse;

    float3 worldPos = GetWorldPosition(uv,depth,mViewProjInv);


    float3 R = camDir.xyz - 2.0f * dot(camDir.xyz, normal) * normal; //反射ベクトル
    
    //float3 li = float3(0.f,0.f,0.f);
    float depthShadow = gshadow;
    float shadow = 0.f;
    float3 foundation = 0.f;
    //最初のライトはデプスシャドウで使っているライトなので別で処理する
    {
        float rad = dot(DirLights[0].dir * -1.f, normal);
        //float threshold = 0.174533f;                        //いずれ未来の自分が改善してくれているはず!
		float k = step(threshold,rad);
        //shadow = 1.0f-k;
        //shadow += depthShadow.x*k;
        float sha = 1.0f-k;
        sha += depthShadow.x*k;

        //li += DirLights[0].color.xyz*k;
        float3 lig = DirLights[0].color.xyz*k;

        float sp = saturate(dot(DirLights[0].dir, R));
        sp *= specular;
        float4 spGradation = texture_1.Sample(Sampler,float2(lerp(0.f,1.f,sp),0.f));  //グラデーションマップ(明かり用)

        foundation = (lig+lig*spGradation.x)*(1.0f-sha);
        shadow = sha;
    }

    //残りのディレクションライトの処理。
    float4 lightres = DirectionLightProcess(normal,specular,shadow,depthShadow,R);
    foundation += lightres.xyz;
    shadow += lightres.w;

    //ポイントライト
    //foundation += PointLightProcess(worldPos,normal,specular);

    col.xyz *= float3(0.45f, 0.4f, 0.6f) + foundation;


    //距離フォグ
    float4 usu = lerp(float4(0.8f, 0.88f, 1.f,1.f),float4(0.f,0.f,0.f,0.f),min(1.f-pow(depth,1000),1));
    //usu *= 1.f-clamp(worldPos.y/3000.f,0.f,1.f);
    //col.xyz *= 1.f-usu.w;
    //col.xyz += usu*usu.w;
    
    return col;
}



float4 DrawProcess_Forward(float3 worldPos,float4 diffuse,float3 normal,float specular,float gshadow,float depth)
{
    float4 col = diffuse;

    


    float3 R = camDir.xyz - 2.0f * dot(camDir.xyz, normal) * normal; //反射ベクトル
    
    float depthShadow = gshadow;
    float shadow = 0.f;
    float3 foundation = 0.f;
    //最初のライトはデプスシャドウで使っているライトなので別で処理する
    {
        float rad = dot(DirLights[0].dir * -1.f, normal);
        //float threshold = 0.174533f;                        //いずれ未来の自分が改善してくれているはず!
		float k = step(threshold,rad);
        float sha = 1.0f-k;
        sha += depthShadow.x*k;

        float3 lig = DirLights[0].color.xyz*k;

        float sp = saturate(dot(DirLights[0].dir, R));
        sp *= specular;
        float4 spGradation = texture_1.Sample(Sampler,float2(lerp(0.f,1.f,sp),0.f));  //グラデーションマップ(明かり用)

        foundation = (lig+lig*spGradation.x)*(1.0f-sha);
        shadow = sha;
    }

    //残りのディレクションライトの処理。
    float4 lightres = DirectionLightProcess(normal,specular,shadow,depthShadow,R);
    foundation += lightres.xyz;
    shadow += lightres.w;

    //ポイントライト
    foundation += PointLightProcess(worldPos,normal,specular);

    col.xyz *= float3(0.45f, 0.4f, 0.6f) + foundation;


    //距離フォグ
    float4 usu = lerp(float4(0.8f, 0.88f, 1.f,1.f),float4(0.f,0.f,0.f,0.f),min(1.f-pow(depth,1000),1));
    //usu *= 1.f-clamp(worldPos.y/3000.f,0.f,1.f);
    //col.xyz *= 1.f-usu.w;
    //col.xyz += usu*usu.w;
    
    return col;
}