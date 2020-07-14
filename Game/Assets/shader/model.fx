/*!
 * @brief	モデルシェーダー。
 */

#include "modelData.h"
#include "Math.h"
//#include "ModelProcess.h"
/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//アルベドテクスチャ。
Texture2D<float4> albedoTexture : register(t0);





float GetShadow(float3 wpos,Texture2D<float4> tex, float2 offset)
{
	float shadow = 0.f;
	float depth = 0.f;
	float isShadow = 0.f;
	float isInUV = 0.f;
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
		isShadow = sign(isShadow+isShadowbuf);
		isInUV = sign(isInUV+isInUVbuf);
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
			int isShadow = !step(depth,shadow.r + depthoffset[i]);
			return float2(1.f*isShadow,(depth-shadow)*isShadow);
		}*/
	}
	//int isShadow = !step(depth,shadow.r + depthoffset[i]);
	return float2(1.f*isShadow,(depth-shadow)*isShadow);
	//return float2(0.f,0.f);
}

/*!
 *@brief	スキン行列を計算。
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
 * @brief	スキンなしモデル用の頂点シェーダー。
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
 * @brief	スキンありモデル用の頂点シェーダー。
 * 全ての頂点に対してこのシェーダーが呼ばれる。
 * Inは1つの頂点データ。VSInputNmTxWeightsを見てみよう。
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
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

	//ハイトマップの読み込み(u~n見た目が変わるだけだから以来ないかも～・・・)
	//psInput.Position += (ps.Normal * hightMap_1.Sample(Sampler,In.TexCoord).x) * hightScale;

	return psInput;
}

/*///////////////////////////////////////////////////////////////////////////////////////////
		スキンなし用の深度値入れる専用の頂点シェーダ
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
		スキンあり用の深度値入れる専用の頂点シェーダ
*///////////////////////////////////////////////////////////////////////////////////////////
PSInputDepth VSMainDepth_Skin(VSInputNmTxWeights In)
{
	PSInputDepth psInput = (PSInputDepth)0;

	//スキン行列を計算。
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
	//Out.specular = float4(In.Pos,1.0f);			//スペキュラマップ
	
	Out.depth = In.PosInProj.z / In.PosInProj.w;

	float2 sdw = GetShadow(In.Pos, shadowMap_1,0);
	Out.shadow = float4(sdw,Out.depth.x*sdw.x,1);//x:影の有無 y:未使用 z:深度値(多分使わない) w:未使用.
	return Out;
}

//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
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
		地面用。
			モデルの拡大縮小によってUVが変形することがないようになっている
*/////////////////////////////////////////////////////////////////////////////////
PSOutput PSMain_Ground(PSInputGround In)
{
	float2 UV = In.TexCoord;
	float3 gsca = groundScale.xyz;	//地面のスケール

	/*
	float3 orig = normalize(mul(groundDir,In.Normal));//回転をなくす

	
	In.TexCoord *= 2.f;

	float x = abs(orig.x);
	float y = abs(orig.y);
	float z = abs(orig.z);
	float o = 0;
	if (x > y && x > z)			//x軸の方を向いている面
	{
		gsca.x = gsca.y;
	}
	else if (y > x && y > z)	//y軸の方を向いている面
	{
		if (orig.y < 0)
		{
			o = gsca.z;
			gsca.z = gsca.x;
			gsca.x = o;
		}
	}
	else if (z > x && z > y)	//z軸の方を向いている面
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

	//アルベドの取得
	float4 alb = float4(0, 0, 0, 1);
	//if (groundUseTexs.x!=0)//ブレンド・S
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

	//ブレンド(テクスチャ2枚)
	len = blend.x + blend.y;
	float2 blendTwoColor = blend.xy;
	blendTwoColor /= len;
	alb1 *= blendTwoColor.x;
	alb2 *= blendTwoColor.y;
	alb.xyz += (alb1.xyz + alb2.xyz) * groundUseTexs.x* !groundUseTexs.w;
	//スペキュラーマップのサンプリング
	specul += (specularMap_1.Sample(Sampler,uvs).x*blendTwoColor.x + specularMap_2.Sample(Sampler,uvs).x*blendTwoColor.y) * groundUseSpes.y* !groundUseSpes.w;


	//ブレンド(テクスチャ3枚)
	float4 alb3 = texture_2.Sample(Sampler, uvs);
	len = blend.x + blend.y + blend.z;
	float3 blendThreeColor = blend.xyz;
	blendThreeColor /= len;
	alb1 *= blendThreeColor.x;
	alb2 *= blendThreeColor.y;
	alb3 *= blendThreeColor.z;
	alb.xyz += (alb1.xyz + alb2.xyz + alb3.xyz)* groundUseTexs.w;

	//スペキュラーマップのサンプリング
	specul += (specularMap_1.Sample(Sampler,uvs).x*blendThreeColor.x + specularMap_2.Sample(Sampler,uvs).x*blendThreeColor.y + specularMap_3.Sample(Sampler,uvs).x*blendThreeColor.z) * groundUseSpes.w;


	//ブレンドなし
	float3 albtex = albedoTexture.Sample(Sampler, uvs);
	alb.xyz += albtex * !groundUseTexs.x;

	//スペキュラーマップのサンプリング
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
		地面用デフォルト
		モデルをほぼそのまま描画する。
*/////////////////////////////////////////////////////////////////////////
PSOutput PSMain_GroundDefault(PSInputGround In)
{
	float2 UV = In.TexCoord;
	float3 gsca = groundScale.xyz;	//地面のスケール

	float2 uvs = UVChanger(UV,gsca,In.NormalFlag,5.0f*0.333333f);

	//アルベドの取得
	float4 alb = float4(0, 0, 0, 1);

	float4 blend = groundBlendMap.Sample(Sampler, UV);
	float4 alb1 = texture_1.Sample(Sampler, uvs);
	float4 alb2 = texture_2.Sample(Sampler, uvs);
	float len = 0.f;

	//ブレンドなし
	float3 albtex = albedoTexture.Sample(Sampler, uvs);
	alb.xyz += albtex;// * !groundUseTexs.x;

	//スペキュラーマップのサンプリング
	float specul = specularMap.Sample(Sampler,uvs).x * groundUseSpes.y;
	specul += 1-groundUseSpes.y;


	PSOutput Out = PSProcess_GBuffer(alb,(PSInput)In);
	Out.specular = specul;
	return Out;
}


/*//////////////////////////////////////////////////////////////////////////
		地面用ブレンド3
		スプラットマップをもとに3枚のテクスチャを合成する
*/////////////////////////////////////////////////////////////////////////
PSOutput PSMain_GroundBlendThree(PSInputGround In)
{
	float2 UV = In.TexCoord;
	float3 gsca = groundScale.xyz;	//地面のスケール

	float2 uvs = UVChanger(UV,gsca,In.NormalFlag,5.0f*10);

	//アルベドの取得
	float4 alb = float4(0, 0, 0, 1);

	
	float4 blend = groundBlendMap.Sample(Sampler, UV);
	float4 alb1 = texture_1.Sample(Sampler, uvs);
	float4 alb2 = texture_2.Sample(Sampler, uvs);
	float4 alb3 = texture_3.Sample(Sampler, uvs);
	float len = 0.f;

	//ブレンド(テクスチャ3枚)
	len = blend.x + blend.y + blend.z;
	float3 blendThreeColor = blend.xyz;
	blendThreeColor /= len;
	alb1 *= blendThreeColor.x;
	alb2 *= blendThreeColor.y;
	alb3 *= blendThreeColor.z;
	alb.xyz += (alb1.xyz + alb2.xyz + alb3.xyz);

	//スペキュラーマップのサンプリング
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
		地面用ブレンド2
		スプラットマップをもとに2枚のテクスチャを合成する
*/////////////////////////////////////////////////////////////////////////
PSOutput PSMain_GroundBlendTwo(PSInputGround In)
{
	float2 UV = In.TexCoord;
	float3 gsca = groundScale.xyz;	//地面のスケール

	float2 uvs = UVChanger(UV,gsca,In.NormalFlag,5.0f);

	//アルベドの取得
	float4 alb = float4(0, 0, 0, 1);

	
	float4 blend = groundBlendMap.Sample(Sampler, UV);
	float4 alb1 = texture_1.Sample(Sampler, uvs);
	float4 alb2 = texture_2.Sample(Sampler, uvs);
	float len = 0.f;

	//ブレンド(テクスチャ2枚)
	len = blend.x + blend.y;
	float2 blendTwoColor = blend.xy;
	blendTwoColor /= len;
	alb1 *= blendTwoColor.x;
	alb2 *= blendTwoColor.y;
	alb.xyz += (alb1.xyz + alb2.xyz);

	//スペキュラーマップのサンプリング
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
		深度値入れるためだけのピクセルシェーダ
*///////////////////////////////////////////////////////////////////////////////
float PSMain_Depth(PSInputDepth In) : SV_Target0
{
	float z = In.PosInProj.z / In.PosInProj.w;
	return z;
}