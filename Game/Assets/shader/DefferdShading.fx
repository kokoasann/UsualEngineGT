
#include "DrawProcess.fx"






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
    float specular = gSpecularMap.Sample(Sampler,In.uv);

    float3 worldPos = GetWorldPosition(In.uv,depth,mViewProjInv);


    float3 R = -camDir.xyz + 2.0f * dot(camDir.xyz, normal) * normal; //反射ベクトル
    
    //float3 li = float3(0.f,0.f,0.f);
    float4 depthShadow = gShadowMap.Sample(Sampler,In.uv);
    float shadow = 0.f;
    float3 foundation = 0.f;
    //最初のライトはデプスシャドウで使っているライトなので別でか処理する
    {
        float rad = dot(DirLights[0].dir * -1.f, normal);
        float threshold = 0.174533f;                        //いずれ未来の自分が改善してくれているはず!
		float k = step(threshold,rad);
        //shadow = 1.0f-k;
        //shadow += depthShadow.x*k;
        float sha = 1.0f-k;
        sha += depthShadow.x*k;

        //li += DirLights[0].color.xyz*k;
        float3 lig = DirLights[0].color.xyz*k;

        float sp = max(0.f,dot(DirLights[0].dir, R));
        sp *= specular;
        float4 spGradation = texture_1.Sample(Sampler,float2(lerp(1.f,0.f,sp),0.f));  //グラデーションマップ(明かり用)

        foundation = (lig+lig*spGradation.x)*(1.0f-sha);
        shadow = sha;
    }
    //残りのディレクションライトの処理。
	for (int i = 1; i < DLcount; i++)
	{
        float rad = dot(DirLights[i].dir * -1.f, normal);
        float threshold = 0.174533f;                        //いずれ未来の自分が改善してくれているはず!
		float k = step(threshold,rad);
        
        //shadow += (1.0f-k)*(1.0f-shadow);
        //li += DirLights[i].color.xyz*k;
        float sha = (1.0f-k)*(1.0f-shadow);
        float3 lig = DirLights[i].color.xyz*k;

        float sp = max(0.f,dot(DirLights[i].dir, R));
        sp *= specular;
        float4 spGradation = texture_1.Sample(Sampler,float2(lerp(1.f,0.f,sp),0.f));  //グラデーションマップ(明かり用)

        foundation += (lig+lig*spGradation.x)*(1.0f-sha);
        shadow += -1.0f * (shadow * (1.0f-sha));
    }
    //ポイントライト
    for(int i=0;i<PLcount;i++)
    {
        float3 lendir = worldPos - PntLights[i].pos;
        float3 dir = normalize(lendir);

        float rad = dot(dir * -1.f, normal);
        float threshold = 0.174533f;                        //いずれ未来の自分が改善してくれているはず!
		float k = step(threshold,rad);
        
        float len = length(lendir);
        float ligPow = max(0.0f,rad) / (1.0f + len);    // V / 1+len
        ligPow *= 10.f;
        ligPow = max(0,min(1.f,ligPow));
        ligPow *= specular;

        float sha = (1.0f-k);

        float colorPow = length(PntLights[i].color.xyz);
        float4 spGradation = texture_1.Sample(Sampler,float2(lerp(1.f,0.f,ligPow),1.f));  //グラデーションマップ(明かり用)
        float3 ligcolor = PntLights[i].color.xyz/len;
        float3 lig = (ligcolor*(spGradation.x));
        
        //反射
        float3 w2p = worldPos - PntLights[i].pos;
        float3 e2w = worldPos - eyepos;
        w2p = normalize(w2p);
        e2w = normalize(e2w);
        float3 w2pRef =  -w2p + 2.0f * dot(w2p,normal) * normal;
        float refDote2w = dot(w2pRef,e2w);
        float sp = max(0,refDote2w);

        //float sp = max(0.f,dot(dir, R));
        sp *= specular;
        spGradation = texture_1.Sample(Sampler,float2(lerp(1.f,0.f,sp),1.f));  //グラデーションマップ(明かり用)

        //foundation += lig+lig*spGradation.x;
        foundation += ligcolor * spGradation.x * k;
    }
    //col.xyz *= float3(0.45f, 0.4f, 0.6f)*shadow;
    col.xyz *= float3(0.45f, 0.4f, 0.6f) + foundation;

    #if 0
    
    float3 v = worldPos-eyepos;
    v=normalize(v);
	float r = dot(v,normal);
	worldPos += normal*r*2.f;
    v = worldPos-eyepos;
    v=normalize(v);
	float sp = max(0,dot(DirLights[0].dir,v));
    #endif

    //depthShadow.x = min(depthShadow.x+shadow,1.f);
    ////depthShadow.x = texture_1.Sample(Sampler,float2(lerp(1.f,0.f,depthShadow.x),0.f)).x;  //グラデーションマップ(影用)
    //li = lerp(float3(0.f,0.f,0.f),float3(0.45f, 0.4f, 0.6f),depthShadow.x);

    //li += (DirLights[0].color.xyz + DirLights[0].color.xyz * spGradation.x)*(1.f-depthShadow.x);
    

    
    float4 usu = lerp(float4(0.8f, 0.88f, 1.f,1.f),float4(0.f,0.f,0.f,0.f),min(lerp(1,0,pow(depth,800)),1));
    col.xyz *= 1.f-usu.w;
    col.xyz += usu;
    
    //return float4(depthShadow.x, depthShadow.x, depthShadow.x,1);
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
float4 PSMain_Specular(DefferdPSInput In):SV_Target0
{
    float s = gSpecularMap.Sample(Sampler,In.uv);
    return float4(s,s,s,1.0f);
}