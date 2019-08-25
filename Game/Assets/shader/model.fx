/*!
 * @brief	���f���V�F�[�_�[�B
 */

#include "modelData.h"
#include "Math.h"

/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//�A���x�h�e�N�X�`���B
Texture2D<float4> albedoTexture : register(t0);	
//�{�[���s��
StructuredBuffer<float4x4> boneMatrix : register(t1);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

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
};


float GetShadow(float3 wpos,Texture2D<float4> tex, float2 offset)
{
	float shadow = 0.f;
	[unroll(MAX_SHADOWMAP)]
	for(int i = 0; i < MAX_SHADOWMAP;i++)
	{
		float4 posinLVP = mul(mLVP[i], float4(wpos, 1));
		posinLVP.xyz /= posinLVP.w;

		float depth = min(posinLVP.z / posinLVP.w, 1.0f);
		float2 smuv = float2(0.5f, -0.5f) * posinLVP.xy + float2(0.5f, 0.5f);
		if (smuv.x < 1.f && smuv.y < 1.f && smuv.x > 0.f && smuv.y > 0.f)
		{
			float2 pix = float2(ligPixSize[i].x * offset.x, ligPixSize[i].y * offset.y);
			shadow = tex.Sample(Sampler, smuv+pix/2).r;
			if (depth > shadow.r+0.0002f)
			{
				return 1.f;
			}
		}
	}
	return 0.f;
}

/*!
 *@brief	�X�L���s����v�Z�B
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll(3)]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}
/*!--------------------------------------------------------------------------------------
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);
	psInput.Pos = pos;
	pos = mul(mView, pos);
	psInput.PosInView = pos;
	pos = mul(mProj, pos);
	psInput.PosInProj = pos;

	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));

	

	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
 * �S�Ă̒��_�ɑ΂��Ă��̃V�F�[�_�[���Ă΂��B
 * In��1�̒��_�f�[�^�BVSInputNmTxWeights�����Ă݂悤�B
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
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

/*///////////////////////////////////////////////////////////////////////////////////////////
		�X�L���Ȃ��p�̐[�x�l������p�̒��_�V�F�[�_
*///////////////////////////////////////////////////////////////////////////////////////////
PSInputDepth VSMainDepth(VSInputNmTxVcTangent In)
{
	PSInputDepth psInput = (PSInputDepth)0;
	float4 pos;
	pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.PosInProj = pos;
	return psInput;
}

/*///////////////////////////////////////////////////////////////////////////////////////////
		�X�L������p�̐[�x�l������p�̒��_�V�F�[�_
*///////////////////////////////////////////////////////////////////////////////////////////
PSInputDepth VSMainDepth_Skin(VSInputNmTxWeights In)
{
	PSInputDepth psInput = (PSInputDepth)0;

	//�X�L���s����v�Z�B
	float4x4 skinning = CalcSkinMatrix(In);

	float4 pos;
	pos = mul(skinning, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.PosInProj = pos;
	return psInput;
}


float4 PSProcess(PSInput In)
{
	float4 albe = albedoTexture.Sample(Sampler, In.TexCoord);

	float2 uv = In.PosInProj.xy / In.PosInProj.w;
	uv *= float2(0.5f, -0.5f);
	uv += 0.5f;
	float sum = 0.f;
	float gaus[5][5] = {
	{1.f / 256.f, 4.f / 256.f,	 6.f / 256.f,   4.f / 256.f,  1.f / 256.f},
	{4.f / 256.f, 16.f / 256.f, 24.f / 256.f, 16.f / 256.f, 4.f / 256.f},
	{6.f / 256.f, 24.f / 256.f, 36.f / 256.f, 24.f / 256.f, 6.f / 256.f},
	{4.f / 256.f, 16.f / 256.f, 24.f / 256.f, 16.f / 256.f, 4.f / 256.f },
	{1.f / 256.f, 4.f / 256.f,   6.f / 256.f,   4.f / 256.f,   1.f / 256.f } };
	int ksize = 5;
	[unroll(ksize)]
	for (int y = 0; y < ksize; y++)
	{
		[unroll(ksize)]
		for (int x = 0; x < ksize; x++)
		{
			sum += GetShadow(In.Pos, shadowMap_1, float2(x - (ksize - 1) / 2, y - (ksize - 1) / 2)) * gaus[y][x];
		}
	}
	float scol = sum;
	//float scol = sum / pow(ksize,2);
	albe.xyz *= lerp(1.0f, 0.5f, scol);

	//albe.xyz *= lerp(1.0f, 0.5f, GetShadow(In.Pos,shadowMap_1,float2(0,0)));

	float3 li = 0.f;
	for (int i = 0; i < DLcount; i++)
	{
		li += max(dot(DirLights[i].dir * -1.f, In.Normal), 0.2f) * DirLights[i].color;
	}
	//albe.xyz *= li;
	float4 fcol = float4(0.f, 0.f, 0.f, 1.f);
	fcol.xyz = albe.xyz;
	return fcol;
}

//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
float4 PSMain(PSInput In) : SV_Target0
{
	return PSProcess(In);
}

/*/////////////////////////////////////////////////////////////////////////////////
		�n�ʗp
*/////////////////////////////////////////////////////////////////////////////////
float4 PSMain_Ground(PSInput In) : SV_Target0
{
	float3 gsca = groundScale;
	float3 gdir = normalize(float3(groundDir.x, 0.f, groundDir.z));
	float deg = Rad2Deg(acos(dot(float3(0, 0, 1), gdir)));
	if (((deg > 45.f && deg < 135.f) || (deg > 180.f && deg < 315.f)))
	{
		float z = gsca.z;
		gsca.z = gsca.x;
		gsca.x = z;
	}
	gdir = normalize(float3(0.f, groundDir.y, groundDir.z));
	deg = Rad2Deg(acos(dot(float3(0, 0, 1), gdir)));
	if (((deg > 45.f && deg < 135.f) || (deg > 180.f && deg < 315.f)))
	{
		float z = gsca.z;
		gsca.z = gsca.y;
		gsca.y = z;
	}
	//In.TexCoord *= 10.f;

	float dg = Rad2Deg(acos(dot(float3(0, 1, 0), In.Normal)));
	if (dg > 45.f && dg < 135.f || dg >180.f && dg < 315.f)
	{
		In.TexCoord.y *= gsca.y;

		dg = Rad2Deg(acos(dot(float3(1,0, 0), In.Normal)));
		if (dg > 45.f && dg < 135.f || dg >180.f && dg < 315.f)
		{
			In.TexCoord.x *= gsca.x;
		}
		else
		{
			In.TexCoord.x *= gsca.z;
		}
	}
	else
	{
		In.TexCoord.x *= gsca.z;
		In.TexCoord.y *= gsca.x;
	}
	
	return PSProcess(In);
}

/*///////////////////////////////////////////////////////////////////////////////
		�[�x�l����邽�߂����̃s�N�Z���V�F�[�_
*///////////////////////////////////////////////////////////////////////////////
float PSMain_Depth(PSInputDepth In) : SV_Target0
{
	float z = In.PosInProj.z / In.PosInProj.w;
	return z;
}