#include "modelData.h"

/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float3 camDir;
	int isShadowReciever;
	int isUseSpecularMap;
};

//インスタンスごとのデータの入った配列。
StructuredBuffer<float4x4> instancingDataList : register(t11);

//	スキンなし用。
PSInput VSMain_NonSkin_Instancing(VSInputNmTxVcTangent In,uint instanceID:SV_InstanceID)
{
    PSInput psInput = (PSInput)0;
	float4 pos = mul(instancingDataList[instanceID], In.Position);
	psInput.Pos = pos;
	pos = mul(mView, pos);
	psInput.PosInView = pos;
	pos = mul(mProj, pos);
	psInput.PosInProj = pos;

	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(instancingDataList[instanceID], In.Normal));
	psInput.Tangent = normalize(mul(instancingDataList[instanceID], In.Tangent));

	return psInput;
}


//	スキンあり用。
PSInput VSMain_Skin_Instancing(VSInputNmTxWeights In,uint instanceID:SV_InstanceID)
{
	PSInput psInput = (PSInput)0;

	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
		[unroll(3)]
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrixにボーン行列が設定されていて、
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //最後のボーンを計算する。
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
		//mulは乗算命令。
	    pos = mul(skinning, In.Position);
	}
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );

	psInput.Pos = pos;
	
	pos = mul(mView, pos);
	psInput.PosInView = pos;
	pos = mul(mProj, pos);
	psInput.PosInProj = pos;

	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
    return psInput;
}


//	スキンなし用デプス
PSInputDepth VSMain_DepthNonSkin_Instancing(VSInputNmTxVcTangent In,uint instanceID:SV_InstanceID)
{
	PSInputDepth psInput = (PSInputDepth)0;
	float4 pos;
	pos = mul(instancingDataList[instanceID], In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.PosInProj = pos;
	return psInput;
}


//	スキンあり用デプス
PSInputDepth VSMain_DepthSkin_Instancing(VSInputNmTxWeights In,uint instanceID:SV_InstanceID)
{
	PSInputDepth psInput = (PSInputDepth)0;

	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll(3)]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);

	float4 pos;
	pos = mul(skinning, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.PosInProj = pos;
	return psInput;
}