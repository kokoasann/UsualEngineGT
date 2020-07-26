#include "modelData.h"
Texture2D<float4> albedoTexture : register(t0);

float4 PSMain_Sky(PSInput In):SV_TARGET0
{
    float4 res = albedoTexture.Sample(Sampler, In.TexCoord);
    float3 dcol = float3(1.,1.,1.0);
    float y = max(In.Pos.y,0.f);
    float rate = min(y/2000000.f,1.f);
    res.xyz = lerp(dcol,res.xyz,rate);
    return res;
}