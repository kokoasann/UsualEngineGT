#include "modelData.h"
#include "PerlinNoise.fxh"
#include "Math.h"
cbuffer CBuffer:register(b7)
{
    float4x4 mVP            :packoffset(c0);//0~3
    float4x4 mVPI           :packoffset(c4);//4~7
    float4   mainLightColor :packoffset(c8);
    float3   mainLightDir   :packoffset(c9);
    float3   camPos         :packoffset(c10);
    float    offset         :packoffset(c10.w);
    float    camFar         :packoffset(c11.x);
    float    camNear        :packoffset(c11.y);
}

#define DEBUG_SHADOWMAP 0
#define SHADOW_RETURN_TYPE float

//depthbuf = GetViewZ(depthbuf,ligFar[ind],ligNear[ind]);
//shadowbuf = GetViewZ(shadowbuf,ligFar[ind],ligNear[ind]);
#define GET_SHADOW(MAPIND)\
 \
    float4 posinLVP = mul(mLVP[ind], float4(wpos, 1)); \
    posinLVP.xyz *= rcp(posinLVP.w);\
    float depthbuf = saturate(posinLVP.z);\
	\
    float2 smuv = mad(float2(0.5f, -0.5f), posinLVP.xy, float2(0.5f, 0.5f));\
\
    float isInUVbuf = (step(smuv.x,1.f)) * (step(smuv.y,1.f)) * (step(0.f,smuv.x)) * (step(0.f,smuv.y));\
    float2 pix = float2(ligPixSize[ind].x * offset.x, ligPixSize[ind].y * offset.y);\
\
    float shadowbuf = shadowMap_##MAPIND.Sample(Sampler, smuv + pix).r;\
	\
    float isShadowbuf = step(depthbuf,shadowbuf+depthoffset[ind]);\
\
    depth += depthbuf * (1.-isInUV)*(isInUVbuf);\
    shadow += shadowbuf * (1.-isInUV)*(isInUVbuf);\
    isShadow += isShadowbuf * (1.-isInUV)*(isInUVbuf);\
    isInUV = sign(isInUV+isInUVbuf);\
    ind++;\

//影分配器。
SHADOW_RETURN_TYPE GetShadow(float3 wpos, float2 offset)
{
	float shadow = 0.f;
	float depth = 0.f;
	float isShadow = 0.f;
	float isInUV = 0.f;
    int ind = 0;
	{
    	GET_SHADOW(1)
	}
	{
    	GET_SHADOW(2)
	}
	{
    	GET_SHADOW(3)
	}

	return float(isShadow+(1.f-isInUV));
}


struct PSInput_RMFog
{
    float4 pos:SV_Position;
    float2 uv:TEXCOORD0;
};

struct PSOutput_RMFog
{
    float4 fog:SV_Target0;
    float4 volume:SV_Target1;
};

#define BINARY_ITERATIONS 8.f
#define BYNARY_DECAY 0.2f

void FogProcess(float3 rayPos,float fogScale,float blendScale,float concentration,float disperse,float fogHeight,
                inout float volume,inout float foundation,inout float fog)
{
    float pernoise = 1.f-PerlinNoise3D(rayPos*fogScale);
    float blend = (1.f-PerlinNoise3D(rayPos*blendScale))*concentration+disperse;

    float hrate = fogHeight/(abs(rayPos.y)+0.1f);
    //float hrate = 1.f-clamp(abs(rayPos.y)/fogHeight,0.f,1.f);
    float f = pernoise * blend;
    f *= hrate;
    float shadowDepth = (GetShadow(rayPos,0.f));
    volume += shadowDepth;

    rayPos += mainLightDir*10.f;
    float ligblend = (1.f-PerlinNoise3D(rayPos*blendScale))*concentration+disperse;

    foundation += (ligblend)*blend;
    fog += f;
}

PSOutput_RMFog _PSMain_RMFog(PSInput_RMFog In)
{
    float gdepth = gDepthMap.Sample(Sampler,In.uv).x;

    float3 fogPos = float3(0,0,0);
    float fogRadius = 1000.f;
    float fogHeight = 50.f;
    
    float concentration = 0.2f;
    float disperse = -0.1f;
    
    #define rayCount 25.f
    float rayStep = 160.f;
    
    #if 0
    //float fogHeight = 200.f;
    //float concentration = 0.7f;
    //float disperse = -0.6f;
    //#define rayCount 60
    //float rayStep = 50.f;
    #endif
    
    

    float fogScale = 0.008f;
    float blendScale = 0.002;

    
    float3 startPos = GetWorldPosition(In.uv,0.f,mVPI);
    float3 endPos = GetWorldPosition(In.uv,0.5f,mVPI);
    float3 s2e = endPos-startPos;
    float3 rayDir = normalize(s2e);


    float foundation = 0.f;
    float fog = 0.f;
    float volume = 0.f;
    float3 col = float3(1.f,1.f,1.f);
    
    #if 0
    {
        float3 rayPos = startPos+rayDir*40.f;
        FogProcess(rayPos,fogScale,blendScale,concentration,disperse,fogHeight,volume,foundation,fog);
    }
    #endif

    float3 rayPos;
    [unroll(rayCount)]
    for(int i=1;i<=rayCount;i++)
    {
        rayPos = startPos+rayDir*rayStep*(float)i;
        {
            float4 vpp =  mul(mVP,float4(rayPos,1.f));
            float dep = vpp.z / vpp.w;
            
            [branch]
            if(dep>gdepth)
            {
                break;
            }
        }

        FogProcess(rayPos,fogScale,blendScale,concentration,disperse,fogHeight,volume,foundation,fog);
    }

    {
        #if 1
        //二分木探索。
        float3 rayAdd = rayDir*rayStep;
        float isHitOld = 1.f;
        float3 slide = rayAdd*-BYNARY_DECAY;
        [unroll(BINARY_ITERATIONS)]
        for(int i=0;i < BINARY_ITERATIONS;i++)
        {
            float isHit;
            rayPos += slide;

            float4 wp = mul(mVP,float4(rayPos,1.0f));
            wp.xyz /= wp.w;
            //float2 hitpix = wp.xy * float2(0.5f, -0.5f) + 0.5f;
            float z = gdepth;
            float dist = z - wp.z;

            isHit = dist > 0.f;

            //treuからfalseもしくはfalseからtrueになった場合折り返す。
            float check = abs(isHitOld - isHit);
            slide *= BYNARY_DECAY*(-1.f*check+(1.f-check));
            isHitOld = isHit;
        }
        FogProcess(rayPos,fogScale,blendScale,concentration,disperse,fogHeight,volume,foundation,fog);
        #endif
    }

    fog = clamp(fog,0.f,0.95f);
    foundation = clamp(foundation,0.f,0.5f);

    col -= lerp(float3(0,0,0),(float3(1.f,1.f,1.f)-float3(0.5f, 0.45f, 0.55f))*1.3f,foundation);

    PSOutput_RMFog Out;
    //Out.fog = float4(col,fog);
    Out.fog = float4(float3(0.9,0.9,0.9),min(fog*0.04f*volume,0.9f));
    Out.volume = mainLightColor*0.03*volume*fog*0.5;
    Out.volume.w *= fog*0.5;
    return Out;
}



float GetViewZ(float depth)
{
    return (camFar*camNear)*rcp(mad((camFar-camNear),depth,-camFar));
}

/*////////////////////////////////////////////////////

*////////////////////////////////////////////////////
PSOutput_RMFog PSMain_RMFog(PSInput_RMFog In)
{
    float gdepth = gDepthMap.Sample(Sampler,In.uv).x;

    float3 fogPos = float3(0,0,0);
    float fogRadius = 1000.f;
    float fogHeight = 100.f;
    
    float concentration = 0.2f;
    float disperse = -0.1f;
    
    #define RAY_COUNT 20.f
    float rayFramePow = 0.4f;
    float rayStep;
    {
        float gView = GetViewZ(gdepth)*-1.f;
        rayStep = (gView)*rcp(RAY_COUNT);
    }

    float fogScale = 0.008f;
    float blendScale = 0.002;

    
    float3 startPos = GetWorldPosition(In.uv,0.f,mVPI);
    float3 endPos = GetWorldPosition(In.uv,0.5f,mVPI);
    float3 s2e = endPos-startPos;
    float3 rayDir = normalize(s2e);


    float foundation = 0.f;
    float fog = 0.f;
    float volume = 0.f;
    float3 col = float3(0.95f,0.95f,0.95f);
    


    float3 rayPos;
    [unroll(RAY_COUNT)]
    for(int i=1;i<=RAY_COUNT;i++)
    {
        float rayLen = rayStep*(float)i;
        rayPos = mad(rayDir, rayLen, startPos);
        float pernoise = 1.f-PerlinNoise3D(rayPos*fogScale);
        float blend = mad((1.f-PerlinNoise3D(rayPos*blendScale)),concentration,disperse);

        float rlRate = 0.003f;
        float hrate = min(fogHeight * rcp(max(rayPos.y*2.f,0.0000001f)),1.f) * saturate(rayLen*rlRate);//clamp(-pow((rayLen-5000.f),2.f)*0.00000001f+1.f,0.f,1.f);
        hrate = saturate(hrate);
        //float hrate = 1.f-clamp(abs(rayPos.y)/fogHeight,0.f,1.f);
        float f = pernoise * blend * hrate;
        //f *= hrate;
        float shadowDepth = (GetShadow(rayPos,0.f));
        volume += shadowDepth;

        float3 rayPosLig = mad(mainLightDir, -5.f, rayPos);
        float ligblend = mad((1.f-PerlinNoise3D(rayPosLig*blendScale)),concentration,disperse);
        hrate = min(fogHeight * rcp(max(rayPosLig.y*2.f,0.0000001f)),1.f) * saturate(length(startPos-rayPosLig) * rlRate);
        hrate = saturate(hrate);

        foundation += (1.f-fog) * ligblend * hrate * f;
        fog += f;
        fog = saturate(fog);
    }
    //volume = clamp(volume,0.f,1.f);
    volume *= 0.05f;    // /20.f
    fog *= 0.8f;
    //fog = clamp(fog,0.f,1.f);
    
    //foundation = clamp(foundation,0.f,1.f);
    //foundation = step(0.08f,foundation)*0.5f;
    foundation *= step(0.01f,foundation)*2.5f;

    //col -= lerp(float3(0,0,0),(float3(1.f,1.f,1.f)-float3(0.5f, 0.45f, 0.55f))*1.3f,step(0.3f,foundation)*0.5f);
    col = lerp(float3(0.95f,0.95f,0.95f),float3(0.5f, 0.45f, 0.55f),foundation);

    PSOutput_RMFog Out;
    Out.fog = float4(col,saturate(fog*volume));
    //Out.volume = mainLightColor*0.03*volume*fog*0.5;
    //Out.volume.w *= fog*0.5;
    return Out;
}