


/*////////////////////////////////////////////////////////////////
		VSInput系.
*////////////////////////////////////////////////////////////////
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
	float4 Position : SV_Position;			//頂点座標。
	float3 Normal : NORMAL;				//法線。
	float3 Tangent : TANGENT;				//接ベクトル。
	float2 TexCoord : TEXCOORD0;			//UV座標。
};

/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
	float4 Position : SV_Position;			//頂点座標。
	float3 Normal : NORMAL;				//法線。
	float2 TexCoord : TEXCOORD0;			//UV座標。
	float3 Tangent : TANGENT;				//接ベクトル。
	uint4  Indices : BLENDINDICES0;		//この頂点に関連付けされているボーン番号。x,y,z,wの要素に入っている。4ボーンスキニング。
	float4 Weights : BLENDWEIGHT0;			//この頂点に関連付けされているボーンへのスキンウェイト。x,y,z,wの要素に入っている。4ボーンスキニング。
};


/*/////////////////////////////////////////////////////////////////////
		PSInput系.
*/////////////////////////////////////////////////////////////////////

/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput {
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 TexCoord : TEXCOORD0;
	float3 Pos : TEXCOORD1;		//ワールド座標
	float3 PosInView : TEXCOORD2;		//カメラ空間での座標
	float4 PosInProj : TEXCOORD3;		//正規化座標系での座標
};

/*
	深度を入れるためだけのピクセルシェーダに入力する構造体
*/
struct PSInputDepth
{
	float4 Position : SV_POSITION;
	float4 PosInProj : TEXCOORD0;			//正規化座標系での座標
};

/*//////////////////////////////////////////////////////////////////////////////////////
	ライト系.
*//////////////////////////////////////////////////////////////////////////////////////

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


/*////////////////////////////////////////////////////////////////////////////////////////
	景彡　系.
*//////////////////////////////////////////////////////////////////////////////////////////
#define MAX_SHADOWMAP 1

cbuffer ShadowCB : register(b3)
{
	float4x4 mLVP[MAX_SHADOWMAP];			//ライトビュープロジェクションマトリクス
	float4 texoffset[MAX_SHADOWMAP];			//オフセット
	float4 ligPixSize[MAX_SHADOWMAP];			//uvでの1ピクセルのサイズ
	float depthoffset[MAX_SHADOWMAP];		//深度オフセット
}

Texture2D<float4> shadowMap_1 : register(t2);		//シャドウマップ


/*/////////////////////////////////////////////////////////////////////////////////////////////
		地面用
*/////////////////////////////////////////////////////////////////////////////////////////////

cbuffer GroundCB : register(b5)
{
	float3 groundScale;				//スケール
}