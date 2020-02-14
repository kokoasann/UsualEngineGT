#include "DrawProcess.fx"

Texture2D<float4> albedoTexture : register(t0);
Texture2D<float4> sceneTexture:register(t13);
Texture2D<float4> alphaMap:register(t12);

float4 PSMain_Alpha(PSInput In):SV_Target0
{
    float depth = In.Position.z / In.Position.w;
    float sceneDep = gDepthMap.Sample(Sampler,In.Position.xy);
    clip(sceneDep-depth);

    float4 res;
    float4 scene = sceneTexture.Sample(Sampler,In.TexCoord+refract(float3(0,0,1),In.Normal,1.5).xy);
    float4 color = albedoTexture.Sample(Sampler, In.TexCoord);
    float alpha = alphaMap.Sample(Sampler,In.TexCoord);
    color.xyz *= alpha;
    scene.xyz *= 1.0f-alpha;
    


    float specular = specularMap.Sample(Sampler,In.TexCoord);

    res = DrawProcess(scene+color,In.Normal,specular,0,depth,In.Position.xy);
    
    return float4(res.xyz,1.0f);
}