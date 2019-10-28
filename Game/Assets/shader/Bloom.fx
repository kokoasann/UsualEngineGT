
struct VSInput
{
	float4 pos:SV_POSITION;
	float2 uv:TEXCOORD0;
};
struct PSInput
{
	float4 pos:SV_POSITION;
	float2 uv:TEXCOORD0;
};

Texture2D<float4> Teture:register(t0);
sampler Sampler; register(s0);

PSInput VSMain(VSInput In)
{
	PSInput ps;
	ps.pos = In.pos;
	ps.uv = In.uv;
	return ps;
}

float4 PSMain_SamplingLuminance(PSInput In) :SV_Target0
{


	float4 col = Texture.Sample(Sampler,In.uv);
	float t = dot(col.xyz, float3(0.2125f, 0.7154f, 0.0721f));
	clip(t - 1.001f);
	col.xyz *= t - 1.f;
	col.w = 1.0f;
	return col;
}

float4 PSMain_Combine(PSInput In):SV_Target0
{
	return float4();
}