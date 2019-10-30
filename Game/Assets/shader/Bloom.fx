
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

Texture2D<float4> Texture:register(t0);
sampler Sampler: register(s0);

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

Texture2D<float4> tex0:register(t0);
Texture2D<float4> tex1:register(t1);
Texture2D<float4> tex2:register(t2);
Texture2D<float4> tex3:register(t3);
Texture2D<float4> tex4:register(t4);

float4 PSMain_Combine(PSInput In) :SV_Target0
{
	float4 col = tex0.Sample(Sampler,In.uv);
	col += tex1.Sample(Sampler, In.uv);
	col += tex2.Sample(Sampler, In.uv);
	col += tex3.Sample(Sampler, In.uv);
	col += tex4.Sample(Sampler, In.uv);
	col /= 5.0f;
	col.w = 1.0f;
	return col;
}