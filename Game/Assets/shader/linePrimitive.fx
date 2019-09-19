
cbuffer LineData:register(b0)
{
	float4x4 mView;
	float4x4 mProj;
}

float4 VSMain(float4 pos:SV_POSITION) :SV_POSITION
{
	float4 outpos = mul(mView,pos);
	outpos = mul(mProj, outpos);
	return outpos;
}

float4 PSMain(float4 pos:SV_POSITION) :SV_TARGET0
{
	return float4(1.f,0.f,0.f,1.f);
}