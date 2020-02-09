#include "modelData.h"

/*!
 * @brief	���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�B
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float3 camDir;
	int isShadowReciever;
	int isUseSpecularMap;
};

//�C���X�^���X���Ƃ̃f�[�^�̓������z��B
StructuredBuffer<float4x4> instancingDataList : register(t11);

//	�X�L���Ȃ��p�B
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


//	�X�L������p�B
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
			//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
			//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
			//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //�Ō�̃{�[�����v�Z����B
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
		//mul�͏�Z���߁B
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


//	�X�L���Ȃ��p�f�v�X
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


//	�X�L������p�f�v�X
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