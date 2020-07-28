#include "modelData.h"

#include "PerlinNoise.fxh"
#include "WorleyNoise.fxh"

cbuffer PSCloudBuffer:register(b7)
{
    float3 campos;
    //float3 camDir;
    float3 mainLightDir;
}

float4 PSMain_PlaneCloud(PSInput In):SV_Target0
{
    //隠れチェック
    float depth = In.PosInProj.z / In.PosInProj.w;
    float2 sceneUV =In.Position.xy;
    sceneUV.x *= 0.00078125f*2.f;// /=1280.f
    sceneUV.y *= 0.00138888f*2.f;// /=720.f
    float sceneDep = gDepthMap.Sample(Sampler,sceneUV);
    clip(sceneDep-depth);

    //こっからが本番。
    float noiseRate = 0.2f;

    float concentration = 0.8f;
    float disperse = -0.7f;

    float altitude = 200.f;
    float height = 50.f;

    #define rayCount 20.f
    float rayStep = 400.f;

    float3 rayDir = normalize(In.Pos - campos);

    float3 altitudeDir = In.Normal*altitude;
    float3 Npos = In.Pos - altitudeDir;
    //float Nmap = (1.f-PerlinNoise3D(Npos))*concentration+disperse;
    
    float3 color = float3(1.f,1.f,1.f);
    float cloud = 0.f;
    [unroll(rayCount)]
    for(int i=0;i<rayCount;i++)
    {
        float3 raypos = Npos+rayDir*rayStep*i;
        float3 sampPos = raypos;
        {
            float pernoise = PerlinNoise3D(sampPos*0.01f);
            float wornoise = 1.f-WorleyNoise3D(sampPos*0.003f);

            float noise = clamp(pernoise*noiseRate + wornoise*(1.f-noiseRate),0.f,1.f);

            float blend = (1.f-PerlinNoise3D(raypos*0.001f))*concentration+disperse;
            cloud += noise*blend;
        }
        //cloud += wornoise*0.1f;

        //ライティング
        raypos += mainLightDir*100.f;
        {
            float pernoise = PerlinNoise3D(sampPos*0.01f);
            float wornoise = 1.f-WorleyNoise3D(sampPos*0.003f);

            float noise = clamp(pernoise*noiseRate + wornoise*(1.f-noiseRate),0.f,1.f);

            float blend = (1.f-PerlinNoise3D(raypos*0.001f))*concentration+disperse;
            color -= lerp(float3(0,0,0),(float3(1.f,1.f,1.f)-float3(0.45f, 0.4f, 0.6f))*0.2f,noise*blend);
        }
    }
    cloud = clamp(cloud,0.f,0.95f);
    //cloud *= 0.8f;
    //clip(cloud-0.0000001f);
    return float4(color,cloud);
}
