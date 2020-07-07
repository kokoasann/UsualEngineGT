
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
    float depthShadow = gShadowMap.Sample(Sampler,In.uv);

    return DrawProcess(diffuse,normal,specular,depthShadow,depth,In.uv);
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