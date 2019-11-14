/*!
 * @brief	モデルシェーダー。
 */

#include "modelData.h"
#include "Math.h"

/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//アルベドテクスチャ。
Texture2D<float4> albedoTexture : register(t0);	
//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float3 camDir;
	int isShadowReciever;
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

			if (depth > shadow.r + depthoffset[i])
			{
				return 1.f;
			}
		}
	}
	return 0.f;
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

float4 PSProcess(float4 albe, PSInput In)
{
	

	float2 uv = In.PosInProj.xy / In.PosInProj.w;
	uv *= float2(0.5f, -0.5f);
	uv += 0.5f;
	float4 shadow = float4(1, 1, 1, 1);
	if (isShadowReciever)
	{
		float sum = 0.f;
		/*float gaus[5][5] = {
		{1.f / 256.f, 4.f / 256.f,	 6.f / 256.f,   4.f / 256.f,  1.f / 256.f},
		{4.f / 256.f, 16.f / 256.f, 24.f / 256.f, 16.f / 256.f, 4.f / 256.f},
		{6.f / 256.f, 24.f / 256.f, 36.f / 256.f, 24.f / 256.f, 6.f / 256.f},
		{4.f / 256.f, 16.f / 256.f, 24.f / 256.f, 16.f / 256.f, 4.f / 256.f },
		{1.f / 256.f, 4.f / 256.f,   6.f / 256.f,   4.f / 256.f,   1.f / 256.f } };*/
		float gaus[3][3] = {
		{1.f / 16.f,2.f / 16.f,1.f / 16.f},
		{2.f / 16.f,4.f / 16.f,2.f / 16.f},
		{1.f / 16.f,2.f / 16.f,1.f / 16.f}
		};
		int ksize = 3;
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
		//float scol = GetShadow(In.Pos, shadowMap_1, float2(0, 0));
		//float scol = sum / pow(ksize,2);

		shadow.x = lerp(1.0f, 0.45f, scol);
		shadow.y = lerp(1.0f, 0.4f, scol);
		shadow.z = lerp(1.0f, 0.6f, scol);
		//albe.xyz *= lerp(1.0f, 0.5f, GetShadow(In.Pos,shadowMap_1,float2(0,0)));
	}


	float3 li = 0.f;
	for (int i = 0; i < DLcount; i++)
	{
		float rad = dot(DirLights[i].dir * -1.f, In.Normal);
		//li += max(rad, 0.2f) * DirLights[i].color;
		if (Rad2Deg(rad) < 10.f)//影の～♪　しょ～りぃ～♪
		{
			li = float3(0.45f, 0.4f, 0.6f);
			
		}
		else//
		{
			li += DirLights[i].color * shadow;

			float3 R = -camDir + 2 * dot(camDir, In.Normal) * In.Normal;
			rad = dot(DirLights[i].dir, R);
			float sp = max(0.f, rad);
			li += pow(sp, 5);
		}
	}
	albe.xyz *= li;

	//遠くをうすく
	float3 usu = float3(0.61f, 0.88f, 1.f) * 0.7f;
	float usulen = min(length(In.PosInView) * 0.00002f, 0.7f);
	//usulen = min(In.PosInView.y * 0.0005f, usulen);
	albe.xyz *= 1.f - usulen;
	albe.xyz += usu * usulen;

	float4 fcol = float4(0.f, 0.f, 0.f, 1.f);
	fcol.xyz = albe.xyz;
	return fcol;
}

//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain(PSInput In) : SV_Target0
{
	float4 albe = albedoTexture.Sample(Sampler, In.TexCoord);
	return PSProcess(albe,In);
}


/*/////////////////////////////////////////////////////////////////////////////////
		地面用。
			モデルの拡大縮小によってUVが変形することがないようになっている
			if文多様につき改良予定(予定)
*/////////////////////////////////////////////////////////////////////////////////
float4 PSMain_Ground(PSInput In) : SV_Target0
{
	float2 UV = In.TexCoord;
	float3 gsca = groundScale.xyz;	//地面のスケール

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

	//アルベドの取得
	float4 alb = float4(0, 0, 0, 1);
	if (groundUseTexs.x!=0)//ブレンド・S
	{
		float4 blend = groundBlendMap.Sample(Sampler, UV);
		float4 alb1 = texture_1.Sample(Sampler, In.TexCoord);
		float4 alb2 = texture_2.Sample(Sampler, In.TexCoord);
		float len = 0.f;
		if(groundUseTexs.w!=0)
		{
			float4 alb3 = texture_2.Sample(Sampler, In.TexCoord);
			len = blend.x + blend.y + blend.z;
			blend /= len;
			alb1 *= blend.x;
			alb2 *= blend.y;
			alb3 *= blend.z;
			alb = alb1 + alb2 + alb3;
		}
		else
		{
			len = blend.x + blend.y;
			blend /= len;
			alb1 *= blend.x;
			alb2 *= blend.y;
			alb = alb1 + alb2;
		}
	}
	else
	{
		alb = albedoTexture.Sample(Sampler, In.TexCoord);
	}

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
}

/*///////////////////////////////////////////////////////////////////////////////
		深度値入れるためだけのピクセルシェーダ
*///////////////////////////////////////////////////////////////////////////////
float PSMain_Depth(PSInputDepth In) : SV_Target0
{
	float z = In.PosInProj.z / In.PosInProj.w;
	return z;
}