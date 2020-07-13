/// <summary>
/// コピー
/// </summary>

struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSOutput
{
	float4 col1:SV_Target0;
	float4 col2:SV_Target1;
};

Texture2D<float4> sceneTexture_1 : register(t0);	//origin
Texture2D<float4> sceneTexture_2 : register(t1);	//origin
sampler Sampler : register(s0);

PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMain(PSInput In) : SV_Target0
{
	return sceneTexture_1.Sample(Sampler, In.uv);
}

PSOutput PSMain_Double(PSInput In)
{
	PSOutput output;
	output.col1 = sceneTexture_1.Sample(Sampler, In.uv);
	output.col2 = sceneTexture_2.Sample(Sampler, In.uv);
	return output;
}