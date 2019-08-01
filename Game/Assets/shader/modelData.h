
struct SDirectionLight
{
	float3 dir;			//ディレクション
	float4 color;		//色
};

StructuredBuffer<SDirectionLight> DirLights : register(t100);

cbuffer LightCB : register(b1)
{
	float3 eyepos : packoffset(c0);			//視点
	int DLcount : packoffset(c0.w);			//ディレクションライトの数
	float4 screenSize : packoffset(c1);		//スクリーンのサイズ
}