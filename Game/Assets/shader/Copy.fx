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

struct PSOutput_2
{
	float4 col1:SV_Target0;
	float4 col2:SV_Target1;
};

struct PSOutput_3
{
	float4 col1:SV_Target0;
	float4 col2:SV_Target1;
	float4 col3:SV_Target2;
};

Texture2D<float4> sceneTexture_1 : register(t0);	//origin
Texture2D<float4> sceneTexture_2 : register(t1);	//origin
Texture2D<float4> sceneTexture_3 : register(t2);	//origin

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

PSOutput_2 PSMain_Double(PSInput In)
{
	PSOutput_2 output;
	output.col1 = sceneTexture_1.Sample(Sampler, In.uv);
	output.col2 = sceneTexture_2.Sample(Sampler, In.uv);
	return output;
}

PSOutput_3 PSMain_Triple(PSInput In)
{
	PSOutput_3 output;
	output.col1 = sceneTexture_1.Sample(Sampler, In.uv);
	output.col2 = sceneTexture_2.Sample(Sampler, In.uv);
	output.col3 = sceneTexture_3.Sample(Sampler, In.uv);
	return output;
}

RWTexture2D<float4> outTexture_1 : register(u0);
cbuffer CopyConstBuffer:register(b0)
{
	float4 whReciprocal;		//コピー先のテクスチャの幅高の逆数(w1,h1,w2,h2)
}

[numthreads(4,4,1)]
void CSMain_Copy(uint3 id:SV_DispatchThreadID)
{
	outTexture_1[id.xy] = sceneTexture_1.SampleLevel(Sampler,float2(id.xy*whReciprocal.xy),0);
}