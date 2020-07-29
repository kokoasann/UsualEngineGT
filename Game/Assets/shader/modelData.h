/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/*////////////////////////////////////////////////////////////////
		VSInput�n.
*////////////////////////////////////////////////////////////////
/*!
 * @brief	�X�L���Ȃ����f���̒��_�\���́B
 */
struct VSInputNmTxVcTangent
{
	float4 Position : SV_Position;			//���_���W�B
	float3 Normal : NORMAL;				//�@���B
	float3 Tangent : TANGENT;				//�ڃx�N�g���B
	float2 TexCoord : TEXCOORD0;			//UV���W�B
};

/*!
 * @brief	�X�L�����胂�f���̒��_�\���́B
 */
struct VSInputNmTxWeights
{
	float4 Position : SV_Position;			//���_���W�B
	float3 Normal : NORMAL;				//�@���B
	float2 TexCoord : TEXCOORD0;			//UV���W�B
	float3 Tangent : TANGENT;				//�ڃx�N�g���B
	uint4  Indices : BLENDINDICES0;		//���̒��_�Ɋ֘A�t������Ă���{�[���ԍ��Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
	float4 Weights : BLENDWEIGHT0;			//���̒��_�Ɋ֘A�t������Ă���{�[���ւ̃X�L���E�F�C�g�Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
};


/*/////////////////////////////////////////////////////////////////////
		PSInput�n.
*/////////////////////////////////////////////////////////////////////

/*!
 * @brief	�s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInput {
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 TexCoord : TEXCOORD0;
	float3 Pos : TEXCOORD1;		//���[���h���W
	float3 PosInView : TEXCOORD2;		//�J������Ԃł̍��W
	float4 PosInProj : TEXCOORD3;		//���K�����W�n�ł̍��W
};

struct PSInputGround {
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 TexCoord : TEXCOORD0;
	float3 Pos : TEXCOORD1;		//���[���h���W
	float3 PosInView : TEXCOORD2;		//�J������Ԃł̍��W
	float4 PosInProj : TEXCOORD3;		//���K�����W�n�ł̍��W
	int4 NormalFlag : TEXCOORD4;
};

/*
	�[�x�����邽�߂����̃s�N�Z���V�F�[�_�ɓ��͂���\����
*/
struct PSInputDepth
{
	float4 Position : SV_POSITION;
	float4 PosInProj : TEXCOORD0;			//���K�����W�n�ł̍��W
};

/*////////////////////////////////////////////////////////////////////////////////////////
	PSOutPut
*////////////////////////////////////////////////////////////////////////////////////////

struct PSOutput
{
	float4 diffuse 	: SV_Target0;
	float4 specular : SV_Target1;
	float4 depth 	: SV_Target2;
	float4 normal 	: SV_Target3;
	float4 shadow 	: SV_Target4;
	float4 tangent 	: SV_Target5;
};

/////////////////////////////////////////////////////////////
// �萔�o�b�t�@�B
/////////////////////////////////////////////////////////////
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
	float refractiveIndex;
};


/*///////////////////////////////////////////////////////////////////////////////////////
//		�{�[���s��
*///////////////////////////////////////////////////////////////////////////////////////
StructuredBuffer<float4x4> boneMatrix : register(t1);




/*////////////////////////////////////////////////////////////////////////////////////////
	�X�y�L�����[
*//////////////////////////////////////////////////////////////////////////////////////////
Texture2D<float4> specularMap:register(t39);
Texture2D<float4> specularMap_1:register(t40);
Texture2D<float4> specularMap_2:register(t41);
Texture2D<float4> specularMap_3:register(t42);


/*////////////////////////////////////////////////////////////////////////////////////////
	height�}�b�v
*//////////////////////////////////////////////////////////////////////////////////////////
Texture2D<float4> hightMap_1:register(t43);


/*////////////////////////////////////////////////////////////////////////////////////////
	�i�c�@�n.
*//////////////////////////////////////////////////////////////////////////////////////////
#define MAX_SHADOWMAP 3

cbuffer ShadowCB : register(b3)
{
	float4x4 mLVP[MAX_SHADOWMAP];			//���C�g�r���[�v���W�F�N�V�����}�g���N�X
	float4 texoffset[MAX_SHADOWMAP];			//�I�t�Z�b�g
	float4 ligPixSize[MAX_SHADOWMAP];			//uv�ł�1�s�N�Z���̃T�C�Y
	float4 depthoffset;		//�[�x�I�t�Z�b�g
	float ligNear[MAX_SHADOWMAP];
	float ligFar[MAX_SHADOWMAP];
}

Texture2D<float4> shadowMap_1 : register(t2);		//�V���h�E�}�b�v
Texture2D<float4> shadowMap_2 : register(t3);		//�V���h�E�}�b�v
Texture2D<float4> shadowMap_3 : register(t4);		//�V���h�E�}�b�v


// G-Buffer
Texture2D<float4> gDiffuseMap : register(t5);	// �f�t���[�Y
Texture2D<float4> gSpecularMap : register(t6);	// �X�y�L����
Texture2D<float4> gDepthMap : register(t7);		// �f�v�X
Texture2D<float4> gNormalMap : register(t8);	// �m�[�}��
Texture2D<float4> gShadowMap : register(t9);	// �V���h�E
Texture2D<float4> gTangentMap : register(t10);	// �^���W�F���g


/*/////////////////////////////////////////////////////////////////////////////////////////////
		�n�ʗp
*/////////////////////////////////////////////////////////////////////////////////////////////

cbuffer GroundCB : register(b5)
{
	float4 groundScale;				//�X�P�[��
	float4x4 groundDir;					//��]�}�g���N�X
	int4 groundUseTexs;
	int4 groundUseSpes;
}
cbuffer GroundCB_VS:register(b6)
{
	int isUseHightMap:packoffset(c0.x);	//�n�C�g�}�b�v���g���H
	float hightScale:packoffset(c0.y);	//�n�C�g�}�b�v�̒l�̃X�P�[��
}
Texture2D<float4> groundBlendMap:register(t101);


Texture2D<float4> texture_1:register(t102);		//���ړI�p
Texture2D<float4> texture_2:register(t103);		//���ړI�p
Texture2D<float4> texture_3:register(t104);		//���ړI�p
