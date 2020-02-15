#include "DrawProcess.fx"

Texture2D<float4> albedoTexture : register(t0);
Texture2D<float4> sceneTexture:register(t13);
Texture2D<float4> alphaMap:register(t12);

float4 PSMain_Alpha(PSInput In):SV_Target0
{
    float depth = In.PosInProj.z / In.PosInProj.w;
    float2 sceneUV =In.Position.xy;
    sceneUV.x /= 1280.f;
    sceneUV.y /= 720.f;
    float sceneDep = gDepthMap.Sample(Sampler,sceneUV);
    clip(sceneDep-depth);

    float4 res;
    float3 ref = refract(camDir,In.Normal,1.);
    float refind = 1.f/1.f;
    float3 eyeline = camDir;
    float dotlignor = dot(eyeline,In.Normal);
    ref = -refind * (eyeline - dotlignor * In.Normal) - In.Normal * sqrt(1 - pow(refind, 2)*(1 - pow(dotlignor, 2)));
    
    float2 uv = sceneUV+ref.xy;
    float4 scene = sceneTexture.Sample(Sampler,uv);
    float4 color = albedoTexture.Sample(Sampler, In.TexCoord);
    float alpha = alphaMap.Sample(Sampler,In.TexCoord);
    color *= 1.0f-alpha;
    scene *= alpha;
    
    float specular = specularMap.Sample(Sampler,In.TexCoord);

    res = DrawProcess(scene+color,In.Normal,specular,0,depth,sceneUV);
    
    return scene+color;
}