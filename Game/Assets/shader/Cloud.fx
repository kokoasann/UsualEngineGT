#include "modelData.h"

#include "PerlinNoise.fxh"
#include "WorleyNoise.fxh"

cbuffer PSCloudBuffer:register(b7)
{
    float3 campos:packoffset(c0);
    //float3 camDir;
    float3 mainLightDir:packoffset(c1);
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
    float rayStep = 100.f;

    float3 rayDir = normalize(In.Pos - campos);

    float3 altitudeDir = In.Normal*altitude;
    float3 Npos = In.Pos - altitudeDir;
    //float Nmap = (1.f-PerlinNoise3D(Npos))*concentration+disperse;

    float pScale = 0.008f;          //perlinノイズのスケール
    float wScale = 0.0025f;         //worleyノイズのスケール
    float distribution = 0.001f;    //雲の分布ノイズのスケール

    float cloudHeight = 600.f;
    
    float3 color = 1.f;
    float cloud = 0.f;
    float foundation = 0.f;
    [unroll(rayCount)]
    for(int i=0;i<rayCount;i++)
    {
        float3 raypos = Npos+rayDir*rayStep*i;
        float blendNoise;
        float3 sampPos = raypos;
        float hrate = min(cloudHeight / max(rayStep*i,0.0000001f),1.f);
        {
            float pernoise = PerlinNoise3D(sampPos*pScale);
            float wornoise = 1.f-WorleyNoise3D(sampPos*wScale);

            float noise = clamp(pernoise*noiseRate + wornoise*(1.f-noiseRate),0.f,1.f);

            float blend = (1.f-PerlinNoise3D(raypos*distribution))*concentration+disperse;
            //float wblend = 1.f-WorleyNoise3D(sampPos*0.0003f)*concentration+disperse;
            //blend = blend*0.5f+wblend*0.5f;
            blendNoise = noise*blend*hrate;
            cloud += blendNoise;
        }
        cloud = clamp(cloud,0.f,1.f);

        //ライティング
        raypos += mainLightDir*-100.f;
        {
            float ligHrate = min(cloudHeight / max(rayStep*i,0.0000001f),1.0f);

            float pernoise = PerlinNoise3D(raypos*pScale);
            float wornoise = 1.f-WorleyNoise3D(raypos*wScale);

            float noise = clamp(pernoise*noiseRate + wornoise*(1.f-noiseRate),0.f,1.f);

            float blend = (1.f-PerlinNoise3D(raypos*distribution))*concentration+disperse;
            //float wblend = 1.f-WorleyNoise3D(raypos*0.0003f)*concentration+disperse;
            //blend = blend*0.5f+wblend*0.5f;
            //color -= lerp(float3(0,0,0),(float3(1.f,1.f,1.f)-float3(0.45f, 0.4f, 0.6f))*2.5f,noise*blend*ligHrate)*blendNoise*(1.f-cloud);
            foundation += noise*blend*ligHrate*blendNoise*(1.f-cloud);
        }
        
    }
    foundation *= 3.f;
    
    //foundation = clamp(foundation,0.f,1.f);
    //foundation *= step(0.07f,foundation*1.f)*5.f;
    foundation = min(foundation,1.f);
    color = lerp(float3(0.95f,0.95f,0.95f), float3(0.4f, 0.4f, 0.6f)*1.f, foundation);
    //cloud *= 0.8f;
    //clip(cloud-0.0000001f);
    return float4(color,cloud);
}
