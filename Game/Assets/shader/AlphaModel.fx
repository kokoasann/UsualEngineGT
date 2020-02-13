#include "modelData.h"

Texture2D<float4> sceneTexture:register(t13);
Texture2D<float4> alphaMap:register(t12);

float4 PSMain_Alpha(PSInput In):SV_Target0
{
    float4 res;
    float4 scene = sceneTexture.Sample(Sampler,In.TexCoord+refract(float3(0,0,1),In.Normal,1.5).xy);
    return res;
}