#include "modelData.h"

struct DefferdVSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};
struct DefferdPSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

DefferdPSInput VSMain_Defferd(DefferdVSInput In)
{
    DefferdPSInput Out;
    Out.pos = In.pos;
    Out.uv = In.uv;
    return Out;
}

float4 PSMain_Defferd(DefferdPSInput In):SV_TARGET0
{
    float4 diffuse = gDiffuseMap.Sample(Sampler,In.uv);
    float4 normal = gNormalMap.Sample(Sampler,In.uv);
    float4 col = diffuse;
    
    float3 li = 0.f;
	for (int i = 0; i < DLcount; i++)
	{
        float rad = dot(DirLights[i].dir * -1.f, normal);
        float threshold = 0.174533f;                        //‚¢‚¸‚ê–¢—ˆ‚ÌŽ©•ª‚ª‰ü‘P‚µ‚Ä‚­‚ê‚Ä‚¢‚é‚Í‚¸!
		int k = step(threshold,rad);

		li = float3(0.45f, 0.4f, 0.6f) * !k;//‰e‚ª•t‚­!
        li += float3(1.f,1.f,1.f)*k;
    }
    col.xyz *= li;
    
    return col;
}