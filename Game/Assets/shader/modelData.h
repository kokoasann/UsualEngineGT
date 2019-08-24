


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

/*
	�[�x�����邽�߂����̃s�N�Z���V�F�[�_�ɓ��͂���\����
*/
struct PSInputDepth
{
	float4 Position : SV_POSITION;
	float4 PosInProj : TEXCOORD0;			//���K�����W�n�ł̍��W
};

/*//////////////////////////////////////////////////////////////////////////////////////
	���C�g�n.
*//////////////////////////////////////////////////////////////////////////////////////

struct SDirectionLight
{
	float3 dir;			//�f�B���N�V����
	float4 color;		//�F
};

StructuredBuffer<SDirectionLight> DirLights : register(t100);

cbuffer LightCB : register(b1)
{
	float3 eyepos : packoffset(c0);			//���_
	int DLcount : packoffset(c0.w);			//�f�B���N�V�������C�g�̐�
	float4 screenSize : packoffset(c1);		//�X�N���[���̃T�C�Y
}


/*////////////////////////////////////////////////////////////////////////////////////////
	�i�c�@�n.
*//////////////////////////////////////////////////////////////////////////////////////////
#define MAX_SHADOWMAP 1

cbuffer ShadowCB : register(b3)
{
	float4x4 mLVP[MAX_SHADOWMAP];			//���C�g�r���[�v���W�F�N�V�����}�g���N�X
	float4 texoffset[MAX_SHADOWMAP];			//�I�t�Z�b�g
	float4 ligPixSize[MAX_SHADOWMAP];			//uv�ł�1�s�N�Z���̃T�C�Y
	float depthoffset[MAX_SHADOWMAP];		//�[�x�I�t�Z�b�g
}

Texture2D<float4> shadowMap_1 : register(t2);		//�V���h�E�}�b�v


/*/////////////////////////////////////////////////////////////////////////////////////////////
		�n�ʗp
*/////////////////////////////////////////////////////////////////////////////////////////////

cbuffer GroundCB : register(b5)
{
	float3 groundScale;				//�X�P�[��
}