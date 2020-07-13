#include "DrawProcess.fx"

Texture2D<float4> albedoTexture : register(t0);
Texture2D<float4> sceneTexture:register(t13);
Texture2D<float4> alphaMap:register(t12);

struct PSOutput_Alpha
{
    float4 OutColor:SV_Target0;
    float4 depth:SV_Target1;
    float4 normal:SV_Target2;
};


PSOutput_Alpha PSMain_Alpha(PSInput In)
{
    float depth = In.PosInProj.z / In.PosInProj.w;
    float2 sceneUV =In.Position.xy;
    sceneUV.x *= 0.00078125f;// /=1280.f
    sceneUV.y *= 0.00138888f;// /=720.f
    float sceneDep = gDepthMap.Sample(Sampler,sceneUV);
    clip(sceneDep-depth);

    float3 norm = In.Normal;
    norm = mul(mView,norm);
    norm = normalize(norm);
    float4 res;
    float3 ref = refract(float3(0,0,1),norm,refractiveIndex);

    float refind = 0.0f;
    float3 eyeline = float3(0,0,1);
    float dotlignor = dot(eyeline,norm);
    //ref = -refractiveIndex * (eyeline - dotlignor * norm) - norm * sqrt(1 - pow(refractiveIndex, 2)*(1 - pow(dotlignor, 2)));
    //dotlignor = dot(camDir,In.Normal);
    //ref = -refractiveIndex * (camDir - dotlignor * In.Normal) - In.Normal * sqrt(1 - pow(refractiveIndex, 2)*(1 - pow(dotlignor, 2)));
    float2 uv = sceneUV+ref.xy;
    /*uv.x -= (uv.x-1.f)*(1.f-step(uv.y,1.f));
    uv.x += (uv.x*-1.f)*(step(uv.x,0.f));
    uv.y -= (uv.y-1.f)*(1.f-step(uv.y,1.f));
    uv.y += (uv.y*-1.f)*(step(uv.y,0.f));*/
    float4 scene = sceneTexture.Sample(Sampler,uv);
    float4 color = albedoTexture.Sample(Sampler, In.TexCoord);
    float alpha = alphaMap.Sample(Sampler,In.TexCoord);
    color *= alpha;
    scene *= 1.0f-alpha;
    
    float specular = specularMap.Sample(Sampler,In.TexCoord);

    PSOutput_Alpha output;
    output.OutColor = DrawProcess(scene+color,In.Normal,1,0,depth,sceneUV);
    output.depth = depth;
    output.normal = float4(In.Normal,1.f);
    return output;
    //return scene+color;
}