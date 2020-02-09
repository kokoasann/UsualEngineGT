#include "modelData.h"
#include "ModelProcess.h"

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