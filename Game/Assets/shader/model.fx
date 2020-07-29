/*!
 * @brief	���f���V�F�[�_�[�B
 */

#include "modelData.h"
#include "Math.h"
//#include "ModelProcess.h"
/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//�A���x�h�e�N�X�`���B
Texture2D<float4> albedoTexture : register(t0);

#define DEBUG_SHADOWMAP 1

//depthbuf = GetViewZ(depthbuf,ligFar[ind],ligNear[ind]);
//shadowbuf = GetViewZ(shadowbuf,ligFar[ind],ligNear[ind]);
#define GET_SHADOW(MAPIND)\
 \
    float4 posinLVP = mul(mLVP[ind], float4(wpos, 1)); \
    posinLVP.xyz /= posinLVP.w;\
    float depthbuf = min(posinLVP.z / posinLVP.w, 1.0f);\
	\
    float2 smuv = float2(0.5f, -0.5f) * posinLVP.xy + float2(0.5f, 0.5f);\
\
    float isInUVbuf = (step(smuv.x,1.f)) * (step(smuv.y,1.f)) * (step(0.f,smuv.x)) * (step(0.f,smuv.y));\
    float2 pix = float2(ligPixSize[ind].x * offset.x, ligPixSize[ind].y * offset.y);\
\
    float shadowbuf = shadowMap_##MAPIND.Sample(Sampler, smuv + pix).r;\
	\
    float isShadowbuf = 1.0f-step(depthbuf,shadowbuf+depthoffset[ind]);\
\
    depth += depthbuf * (1.0f-isShadow)* isShadowbuf;\
    shadow += shadowbuf * (1.0f-isShadow)* isShadowbuf;\
    isShadow += isShadowbuf * (1.-isInUV)*(isInUVbuf);\
    isInUV = sign(isInUV+isInUVbuf);\
    ind++;\


float4 GetShadow(float3 wpos,Texture2D<float4> tex, float2 offset)
{
	float shadow = 0.f;
	float depth = 0.f;
	float isShadow = 0.f;
	float isInUV = 0.f;
    int ind = 0;
	#if DEBUG_SHADOWMAP
	float3 col = float3(0,0,0);
	{
    GET_SHADOW(1)
	if(isInUVbuf)
	{
		col.r = 0.5;
	}
	}
	{
    GET_SHADOW(2)
	if(isInUVbuf)
	{
		col.g = 0.5;
	}
	}
	{
    GET_SHADOW(3)
	if(isInUVbuf)
	{
		col.b = 0.5;
	}
	}
	#endif

	#if DEBUG_SHADOWMAP
	return float4(isShadow,col);
	#else
	return float(isShadow);
	#endif
}


float4 _GetShadow(float3 wpos,Texture2D<float4> tex, float2 offset)
{
	float shadow = 0.f;
	float depth = 0.f;
	float isShadow = 0.f;
	float isInUV = 0.f;
	float3 col = float3(0,0,0);
	[unroll(MAX_SHADOWMAP)]
	for(int i = 0; i < MAX_SHADOWMAP;i++)
	{
		float4 posinLVP = mul(mLVP[i], float4(wpos, 1));
		posinLVP.xyz /= posinLVP.w;

		float depthbuf = min(posinLVP.z / posinLVP.w, 1.0f);
		float2 smuv = float2(0.5f, -0.5f) * posinLVP.xy + float2(0.5f, 0.5f);

		float isInUVbuf = (1.0f-step(1.f,smuv.x)) * (1.0f-step(1.f,smuv.y)) * (1.0f-step(smuv.x,0.f)) * (1.0f-step(smuv.y,0.f));
		float2 pix = float2(ligPixSize[i].x * offset.x, ligPixSize[i].y * offset.y);

		float shadowbuf = 0.f;
		float is_inuvbuf_shadow_isuv = isInUVbuf * (1.0f - isShadow) * (1.0f-isInUV);
		shadowbuf += shadowMap_1.Sample(Sampler, smuv + pix * 0.5f).r * is_inuvbuf_shadow_isuv * (1.0f-abs(sign(0-i)));
		shadowbuf += shadowMap_2.Sample(Sampler, smuv + pix * 0.5f).r * is_inuvbuf_shadow_isuv * (1.0f-abs(sign(1-i)));
		shadowbuf += shadowMap_3.Sample(Sampler, smuv + pix * 0.5f).r * is_inuvbuf_shadow_isuv * (1.0f-abs(sign(2-i)));
		float isShadowbuf = (1.0f-step(depthbuf,shadowbuf + depthoffset[i]))*isInUVbuf*sign(shadowbuf);
		//depth += depthbuf * isShadowbuf;
		shadow += shadowbuf*(1.0f-isShadow)* isShadowbuf;
		isShadow += isShadowbuf * (1.-isInUV)*(isInUVbuf);
		isInUV = sign(isInUV+isInUVbuf);

		#if DEBUG_SHADOWMAP
		if(isInUVbuf)
		{
		if (i == 0)
		{
			col.x = 0.5;
		}
		else if (i == 1)
		{
			col.y = 0.5;
		}
		else if (i == 2)
		{
			col.z = 0.5;
		}
		}
		#endif
		/*
		if (smuv.x < 1.f && smuv.y < 1.f && smuv.x > 0.f && smuv.y > 0.f)
		{
			float2 pix = float2(ligPixSize[i].x * offset.x, ligPixSize[i].y * offset.y);
			if (i == 0)
			{
				shadow = shadowMap_1.Sample(Sampler, smuv + pix / 2).r;
			}
			else if (i == 1)
			{
				shadow = shadowMap_2.Sample(Sampler, smuv + pix / 2).r;
			}
			else if (i == 2)
			{
				shadow = shadowMap_3.Sample(Sampler, smuv + pix / 2).r;
			}
			int isShadow = 1.f-step(depth,shadow.r + depthoffset[i]);
			return float2(1.f*isShadow,(depth-shadow)*isShadow);
		}*/
	}
	//int isShadow = !step(depth,shadow.r + depthoffset[i]);
	//return float2(1.f*isShadow,(depth-shadow)*isShadow);
	#if DEBUG_SHADOWMAP
	return float4(1.f*isShadow,col);
	#else
	return float(isShadow);
	#endif
	//return float2(0.f,0.f);
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

PSInputGround VSMain_Ground(VSInputNmTxVcTangent In)
{
	PSInputGround psInput = (PSInputGround)0;

	

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

	float ax=abs(In.Normal.x),ay=abs(In.Normal.y),az=abs(In.Normal.z);
	psInput.NormalFlag.x = step(ay,ax)*step(az,ax);
	psInput.NormalFlag.y = step(ax,ay)*step(az,ay);
	psInput.NormalFlag.z = step(ay,az)*step(ax,az)*step(0,In.Normal.z);

	//�n�C�g�}�b�v�̓ǂݍ���(u~n�����ڂ��ς�邾��������ȗ��Ȃ������`�E�E�E)
	//psInput.Position += (ps.Normal * hightMap_1.Sample(Sampler,In.TexCoord).x) * hightScale;

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

PSOutput PSProcess_GBuffer(float4 albe,PSInput In)
{
	PSOutput Out = (PSOutput)0;
	Out.diffuse = albe;
	Out.normal = float4(In.Normal,1.0f);
	Out.tangent = float4(In.Tangent,1.0f);
	//Out.specular = float4(In.Pos,1.0f);			//�X�y�L�����}�b�v
	
	Out.depth = In.PosInProj.z / In.PosInProj.w;
	
	#if DEBUG_SHADOWMAP
	float4 sdw = GetShadow(In.Pos, shadowMap_1,0);
	Out.diffuse.xyz += sdw.yzw;
	Out.shadow = float4(sdw.xy,Out.depth.x*sdw.x,1);//x:�e�̗L�� y:���g�p z:�[�x�l(�����g��Ȃ�) w:���g�p.
	#else
	float sdw = GetShadow(In.Pos, shadowMap_1,0);
	Out.shadow = float4(sdw.x,0,Out.depth.x*sdw.x,1);//x:�e�̗L�� y:���g�p z:�[�x�l(�����g��Ȃ�) w:���g�p.
	#endif
	return Out;
}

//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
PSOutput PSMain(PSInput In)
{
	float4 albe = albedoTexture.Sample(Sampler, In.TexCoord);
	//return PSProcess(albe,In);
	PSOutput Out = PSProcess_GBuffer(albe,In);
	Out.specular = specularMap.Sample(Sampler,In.TexCoord)*isUseSpecularMap;
	//Out.specular += 1 - isUseSpecularMap;
	return Out; 
}





/*/////////////////////////////////////////////////////////////////////////////////
		�n�ʗp�B
			���f���̊g��k���ɂ����UV���ό`���邱�Ƃ��Ȃ��悤�ɂȂ��Ă���
*/////////////////////////////////////////////////////////////////////////////////
PSOutput PSMain_Ground(PSInputGround In)
{
	float2 UV = In.TexCoord;
	float3 gsca = groundScale.xyz;	//�n�ʂ̃X�P�[��

	/*
	float3 orig = normalize(mul(groundDir,In.Normal));//��]���Ȃ���

	
	In.TexCoord *= 2.f;

	float x = abs(orig.x);
	float y = abs(orig.y);
	float z = abs(orig.z);
	float o = 0;
	if (x > y && x > z)			//x���̕��������Ă����
	{
		gsca.x = gsca.y;
	}
	else if (y > x && y > z)	//y���̕��������Ă����
	{
		if (orig.y < 0)
		{
			o = gsca.z;
			gsca.z = gsca.x;
			gsca.x = o;
		}
	}
	else if (z > x && z > y)	//z���̕��������Ă����
	{
		gsca.z = gsca.x;
		gsca.x = gsca.y;
	}
	In.TexCoord.x *= gsca.z;
	In.TexCoord.y *= gsca.x;
	*/

	float2 uvs=float2(0,0);

	//Z Axis
	uvs.x += UV.x * gsca.z * In.NormalFlag.z;
	uvs.y += UV.y * gsca.y * In.NormalFlag.z;

	//X Axis
	uvs.x += UV.x * gsca.z * In.NormalFlag.x;
	uvs.y += UV.y * gsca.y * In.NormalFlag.x;

	//Y Axis
	uvs.x += UV.x * gsca.y * In.NormalFlag.y;
	uvs.y += UV.y * gsca.x * In.NormalFlag.y;

	uvs += UV.xy * gsca.xz * !(In.NormalFlag.x + In.NormalFlag.y + In.NormalFlag.z);
	uvs *= 5.0f;
	//uvs *= 2.0f;

	//�A���x�h�̎擾
	float4 alb = float4(0, 0, 0, 1);
	//if (groundUseTexs.x!=0)//�u�����h�ES
	//{
	//	float4 blend = groundBlendMap.Sample(Sampler, UV);
	//	float4 alb1 = texture_1.Sample(Sampler, In.TexCoord);
	//	float4 alb2 = texture_2.Sample(Sampler, In.TexCoord);
	//	float len = 0.f;
	//	if(groundUseTexs.w!=0)
	//	{
	//		float4 alb3 = texture_2.Sample(Sampler, In.TexCoord);
	//		len = blend.x + blend.y + blend.z;
	//		blend /= len;
	//		alb1 *= blend.x;
	//		alb2 *= blend.y;
	//		alb3 *= blend.z;
	//		alb = alb1 + alb2 + alb3;
	//	}
	//	else
	//	{
	//		len = blend.x + blend.y;
	//		blend /= len;
	//		alb1 *= blend.x;
	//		alb2 *= blend.y;
	//		alb = alb1 + alb2;
	//	}
	//}
	//else
	//{
	//	alb = albedoTexture.Sample(Sampler, In.TexCoord);
	//}
	float specul = 0.0f;
	
	float4 blend = groundBlendMap.Sample(Sampler, UV);
	float4 alb1 = texture_1.Sample(Sampler, uvs);
	float4 alb2 = texture_2.Sample(Sampler, uvs);
	float len = 0.f;

	//�u�����h(�e�N�X�`��2��)
	len = blend.x + blend.y;
	float2 blendTwoColor = blend.xy;
	blendTwoColor /= len;
	alb1 *= blendTwoColor.x;
	alb2 *= blendTwoColor.y;
	alb.xyz += (alb1.xyz + alb2.xyz) * groundUseTexs.x* !groundUseTexs.w;
	//�X�y�L�����[�}�b�v�̃T���v�����O
	specul += (specularMap_1.Sample(Sampler,uvs).x*blendTwoColor.x + specularMap_2.Sample(Sampler,uvs).x*blendTwoColor.y) * groundUseSpes.y* !groundUseSpes.w;


	//�u�����h(�e�N�X�`��3��)
	float4 alb3 = texture_2.Sample(Sampler, uvs);
	len = blend.x + blend.y + blend.z;
	float3 blendThreeColor = blend.xyz;
	blendThreeColor /= len;
	alb1 *= blendThreeColor.x;
	alb2 *= blendThreeColor.y;
	alb3 *= blendThreeColor.z;
	alb.xyz += (alb1.xyz + alb2.xyz + alb3.xyz)* groundUseTexs.w;

	//�X�y�L�����[�}�b�v�̃T���v�����O
	specul += (specularMap_1.Sample(Sampler,uvs).x*blendThreeColor.x + specularMap_2.Sample(Sampler,uvs).x*blendThreeColor.y + specularMap_3.Sample(Sampler,uvs).x*blendThreeColor.z) * groundUseSpes.w;


	//�u�����h�Ȃ�
	float3 albtex = albedoTexture.Sample(Sampler, uvs);
	alb.xyz += albtex * !groundUseTexs.x;

	//�X�y�L�����[�}�b�v�̃T���v�����O
	specul += specularMap_1.Sample(Sampler,uvs).x * groundUseSpes.y;
	
	specul += 1-groundUseSpes.y;

	/*
	float4 fcol = PSProcess(alb,In);

	float rad = acos(dot(camDir * -1.f, In.Normal));
	rad = Rad2Deg(rad);
	rad = min(rad, 360.f - rad);
	rad = lerp(0, 0.4f, rad / 180.f);
	rad *= lerp(1,0,length(In.PosInView) * 0.00007f);
	float3 usu = float3(0.81f, 0.88f, 0.5f) * 0.7f;
	float usulen = rad;
	//usulen = min(In.PosInView.y * 0.0005f, usulen);
	fcol.xyz *= 1.f - usulen;
	fcol.xyz += usu * usulen;
	
	return fcol;
	*/
	
	PSOutput Out = PSProcess_GBuffer(alb,(PSInput)In);
	Out.specular = specul;
	return Out;
}

float2 UVChanger(float2 UV,float3 groundScale,int4 NormalFlag,float offset)
{
	float2 uvs=float2(0,0);

	//Z Axis
	uvs.x += UV.x * groundScale.z * NormalFlag.z;
	uvs.y += UV.y * groundScale.y * NormalFlag.z;

	//X Axis
	uvs.x += UV.x * groundScale.z * NormalFlag.x;
	uvs.y += UV.y * groundScale.y * NormalFlag.x;

	//Y Axis
	uvs.x += UV.x * groundScale.y * NormalFlag.y;
	uvs.y += UV.y * groundScale.x * NormalFlag.y;

	uvs += UV.xy * groundScale.xz * !(NormalFlag.x + NormalFlag.y + NormalFlag.z);
	uvs *= offset;
	
	return uvs;
}

/*//////////////////////////////////////////////////////////////////////////
		�n�ʗp�f�t�H���g
		���f�����قڂ��̂܂ܕ`�悷��B
*/////////////////////////////////////////////////////////////////////////
PSOutput PSMain_GroundDefault(PSInputGround In)
{
	float2 UV = In.TexCoord;
	float3 gsca = groundScale.xyz;	//�n�ʂ̃X�P�[��

	float2 uvs = UVChanger(UV,gsca,In.NormalFlag,5.0f*0.333333f);

	//�A���x�h�̎擾
	float4 alb = float4(0, 0, 0, 1);

	float4 blend = groundBlendMap.Sample(Sampler, UV);
	float4 alb1 = texture_1.Sample(Sampler, uvs);
	float4 alb2 = texture_2.Sample(Sampler, uvs);
	float len = 0.f;

	//�u�����h�Ȃ�
	float3 albtex = albedoTexture.Sample(Sampler, uvs);
	alb.xyz += albtex;// * !groundUseTexs.x;

	//�X�y�L�����[�}�b�v�̃T���v�����O
	float specul = specularMap.Sample(Sampler,uvs).x * groundUseSpes.y;
	specul += 1-groundUseSpes.y;


	PSOutput Out = PSProcess_GBuffer(alb,(PSInput)In);
	Out.specular = specul;
	return Out;
}


/*//////////////////////////////////////////////////////////////////////////
		�n�ʗp�u�����h3
		�X�v���b�g�}�b�v�����Ƃ�3���̃e�N�X�`������������
*/////////////////////////////////////////////////////////////////////////
PSOutput PSMain_GroundBlendThree(PSInputGround In)
{
	float2 UV = In.TexCoord;
	float3 gsca = groundScale.xyz;	//�n�ʂ̃X�P�[��

	float2 uvs = UVChanger(UV,gsca,In.NormalFlag,5.0f*10);

	//�A���x�h�̎擾
	float4 alb = float4(0, 0, 0, 1);

	
	float4 blend = groundBlendMap.Sample(Sampler, UV);
	float4 alb1 = texture_1.Sample(Sampler, uvs);
	float4 alb2 = texture_2.Sample(Sampler, uvs);
	float4 alb3 = texture_3.Sample(Sampler, uvs);
	float len = 0.f;

	//�u�����h(�e�N�X�`��3��)
	len = blend.x + blend.y + blend.z;
	float3 blendThreeColor = blend.xyz;
	blendThreeColor /= len;
	alb1 *= blendThreeColor.x;
	alb2 *= blendThreeColor.y;
	alb3 *= blendThreeColor.z;
	alb.xyz += (alb1.xyz + alb2.xyz + alb3.xyz);

	//�X�y�L�����[�}�b�v�̃T���v�����O
	float specul1 = specularMap_1.Sample(Sampler,uvs).x*groundUseSpes.y;
	specul1 += 1 - groundUseSpes.y;
	float specul2 = specularMap_2.Sample(Sampler,uvs).x*groundUseSpes.z;
	specul2 += 1 - groundUseSpes.z;
	float specul3 = specularMap_3.Sample(Sampler,uvs).x*groundUseSpes.w;
	specul3 += 1 - groundUseSpes.w;

	float specul = (specul1*blendThreeColor.x + specul2*blendThreeColor.y + specul3*blendThreeColor.z);

	PSOutput Out = PSProcess_GBuffer(alb,(PSInput)In);
	Out.specular = specul;
	return Out;
}


/*//////////////////////////////////////////////////////////////////////////
		�n�ʗp�u�����h2
		�X�v���b�g�}�b�v�����Ƃ�2���̃e�N�X�`������������
*/////////////////////////////////////////////////////////////////////////
PSOutput PSMain_GroundBlendTwo(PSInputGround In)
{
	float2 UV = In.TexCoord;
	float3 gsca = groundScale.xyz;	//�n�ʂ̃X�P�[��

	float2 uvs = UVChanger(UV,gsca,In.NormalFlag,5.0f);

	//�A���x�h�̎擾
	float4 alb = float4(0, 0, 0, 1);

	
	float4 blend = groundBlendMap.Sample(Sampler, UV);
	float4 alb1 = texture_1.Sample(Sampler, uvs);
	float4 alb2 = texture_2.Sample(Sampler, uvs);
	float len = 0.f;

	//�u�����h(�e�N�X�`��2��)
	len = blend.x + blend.y;
	float2 blendTwoColor = blend.xy;
	blendTwoColor /= len;
	alb1 *= blendTwoColor.x;
	alb2 *= blendTwoColor.y;
	alb.xyz += (alb1.xyz + alb2.xyz);

	//�X�y�L�����[�}�b�v�̃T���v�����O
	float specul1 = specularMap_1.Sample(Sampler,uvs).x*groundUseSpes.y;
	specul1 += 1 - groundUseSpes.y;
	float specul2 = specularMap_2.Sample(Sampler,uvs).x*groundUseSpes.z;
	specul2 += 1 - groundUseSpes.z;
	float specul = (specul1*blendTwoColor.x + specul2*blendTwoColor.y);

	PSOutput Out = PSProcess_GBuffer(alb,(PSInput)In);
	Out.specular = specul;
	return Out;
}


/*///////////////////////////////////////////////////////////////////////////////
		�[�x�l����邽�߂����̃s�N�Z���V�F�[�_
*///////////////////////////////////////////////////////////////////////////////
float PSMain_Depth(PSInputDepth In) : SV_Target0
{
	float z = In.PosInProj.z / In.PosInProj.w;
	return z;
}