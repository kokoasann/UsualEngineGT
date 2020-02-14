

/*//////////////////////////////////////////////////////////////////////////////////////
	ライト系.
*//////////////////////////////////////////////////////////////////////////////////////

struct SDirectionLight
{
	float3 dir;			//ディレクション
	float4 color;		//色
};

StructuredBuffer<SDirectionLight> DirLights : register(t50);

struct SPointLight
{
	float3 pos;
	float4 color;
};

StructuredBuffer<SPointLight> PntLights : register(t51);

cbuffer LightCB : register(b1)
{
	float4 screenSize : packoffset(c0);		//スクリーンのサイズ
	float3 eyepos : packoffset(c1);			//視点
	int DLcount : packoffset(c1.w);			//ディレクションライトの数
	int PLcount : packoffset(c2.x);			//ポイントライトの数
}