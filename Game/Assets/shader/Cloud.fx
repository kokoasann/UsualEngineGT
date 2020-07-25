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
    float noiseRate = 0.2f;

    float concentration = 0.4f;
    float disperse = -0.4f;

    float altitude = 200.f;
    float height = 50.f;

    #define rayCount 20.f
    float rayStep = 400.f;

    float3 rayDir = normalize(In.Pos - campos);

    float3 altitudeDir = In.Normal*altitude;
    float3 Npos = In.Pos - altitudeDir;
    //float Nmap = (1.f-PerlinNoise3D(Npos))*concentration+disperse;
    
    float cloud = 0.f;
    [unroll(rayCount)]
    for(int i=0;i<rayCount;i++)
    {
        float3 raypos = Npos+rayDir*rayStep*i;
        float3 sampPos = raypos;

        float pernoise = PerlinNoise3D(sampPos*0.01f);
        float wornoise = 1.f-WorleyNoise3D(sampPos*0.003f);

        float noise = clamp(pernoise*noiseRate + wornoise*(1.f-noiseRate),0.f,1.f);

        float blend = (1.f-PerlinNoise3D(raypos*0.001f))*concentration+disperse;
        cloud += noise*blend;
        //cloud += wornoise*0.1f;
    }
    cloud = clamp(cloud,0.f,0.95f);
    //cloud *= 0.8f;
    //clip(cloud-0.0000001f);
    return float4(1.f,1.f,1.f,cloud);
}