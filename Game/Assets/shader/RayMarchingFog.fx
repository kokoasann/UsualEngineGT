#include "modelData.h"
#include "PerlinNoise.fxh"
#include "Math.h"
cbuffer CBuffer:register(b7)
{
    float4x4 mVP;
    float4x4 mVPI;
    float4 mainLightColor;
    float3 mainLightDir;
    float3 camPos;
    float offset;
}

#define DEBUG_SHADOWMAP 0
#define SHADOW_RETURN_TYPE float

//depthbuf = GetViewZ(depthbuf,ligFar[ind],ligNear[ind]);
//shadowbuf = GetViewZ(shadowbuf,ligFar[ind],ligNear[ind]);
#define GET_SHADOW(MAPIND)\
 \
    float4 posinLVP = mul(mLVP[ind], float4(wpos, 1)); \
    posinLVP.xyz /= posinLVP.w;\
    float depthbuf = min(posinLVP.z / posinLVP.w, 1.0f);\
	\
    float2 smuv = float2(0.5f, -0.5f) * posinLVP.xy + float2(0.5f, 0.5f);\
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

	return float(isShadow);
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

PSOutput_RMFog PSMain_RMFog(PSInput_RMFog In)
{
    float gdepth = gDepthMap.Sample(Sampler,In.uv).x;

    float3 fogPos = float3(0,0,0);
    float fogRadius = 1000.f;
    float fogHeight = 50.f;
    
    float concentration = 0.2f;
    float disperse = -0.1f;
    
    #define rayCount 50.f
    float rayStep = 80.f;
    
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
    
    {
        float3 rayPos = startPos+rayDir*40.f;
        FogProcess(rayPos,fogScale,blendScale,concentration,disperse,fogHeight,volume,foundation,fog);
    }
    
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
    Out.fog = float4(float3(0.9,0.9,0.9),min(fog*0.04*volume,0.9f));
    Out.volume = mainLightColor*0.03*volume*fog*0.5;
    Out.volume.w *= fog*0.5;
    return Out;
}