#include "lightData.h"

#define MAX_LIGHT 32
#define TILE_WIDTH 16
#define TILE_SIZE (TILE_WIDTH*TILE_WIDTH)

sampler Sampler : register(s0);


//カメラの情報。
cbuffer cbCameraParam : register( b0 )
{
	float4x4	mtxProj				: packoffset( c0 );		// 投影行列
	float4x4 	mtxProjInv			: packoffset( c4 );		// 投影行列の逆行列。
	float4x4	mtxViewRot			: packoffset( c8 );
	float4		screenParam			: packoffset( c12 );	// スクリーンパラメータ(near, far, screenWidth, screenHeight)
};


//ディフューズテクスチャ。
Texture2D diffuseTexture : register(t0);
//法線テクスチャ。
Texture2D normalTexture : register(t1);
//深度テクスチャ。
Texture2D depthTexture : register(t2);
//スペキュラテクスチャ。
Texture2D specularTexture : register(t3);

//トゥーンシェード用のグラデーション。
Texture2D<float4> gradationTexture:register(t102);

//出力テクスチャ。
RWTexture2D<float4> rwTexture : register(u0);

//共有メモリ。
groupshared uint sMinZ;		//タイルの最小深度。
groupshared uint sMaxZ;		//タイルの最大深度。
groupshared uint sTileLightIndices[MAX_LIGHT];	//タイルに接触しているポイントライトのインデックス。
groupshared uint sTileNumLights;				//タイルに接触しているポイントライトの数。
groupshared uint sPerSamplePixels[TILE_SIZE];
groupshared uint sNumPerSamplePixels;


void GetTileFrustumPlane( out float4 frustumPlanes[6], uint3 groupId )
{
	// タイルの最大・最小深度を浮動小数点に変換
	float minTileZ = asfloat(sMinZ);
	float maxTileZ = asfloat(sMaxZ);


	float2 tileScale = screenParam.zw * rcp( float(2 * TILE_WIDTH) );
	float2 tileBias = tileScale - float2(groupId.xy);

	float4 c1 = float4(mtxProj._11 * tileScale.x, 0.0, tileBias.x, 0.0);
	float4 c2 = float4(0.0, -mtxProj._22 * tileScale.y, tileBias.y, 0.0);
	float4 c4 = float4(0.0, 0.0, 1.0, 0.0);

	frustumPlanes[0] = c4 - c1;		// Right
	frustumPlanes[1] = c4 + c1;		// Left
	frustumPlanes[2] = c4 - c2;		// Top
	frustumPlanes[3] = c4 + c2;		// Bottom
	frustumPlanes[4] = float4(0.0, 0.0, 1.0, -minTileZ);	//near
	frustumPlanes[5] = float4(0.0, 0.0, -1.0, maxTileZ);	//far

	// 法線が正規化されていない4面についてだけ正規化する
	[unroll(4)]
	for (uint i = 0; i < 4; ++i)
	{
		frustumPlanes[i] *= rcp( length( frustumPlanes[i].xyz ) );
	}
}


float3 ComputePositionInCamera( uint2 globalCoords )
{
	float2	st = ((float2)globalCoords + 0.5) * rcp( screenParam.zw );
	st = st * float2( 2.0, -2.0 ) - float2( 1.0, -1.0 );
	float3 screenPos;
	screenPos.xy = st.xy;
	screenPos.z = depthTexture.Load(uint3(globalCoords, 0.0f)).x;
	float4 cameraPos = mul(mtxProjInv, float4(screenPos, 1.0f));
	
	return cameraPos.xyz * rcp(cameraPos.w);
}


float3 PointLightProcess(uint lightIndex, float3 posInView, float3 normal, float specular)
{
	SPointLight light = PntLights[lightIndex];

	float3 foundation;

	float3 dir = posInView - light.posInView;
	float len = length(dir);
	dir = normalize(dir);

	float rad = dot(dir * -1.f, normal);
	float threshold = 0.5f;                        //いずれ未来の自分が改善してくれているはず!
	float k = step(threshold,rad);
	
	k = min(k, step(len,light.radius));

	float ligPow = len*light.decay;

	//float ligDecay = gradationTexture[uint2(saturate(ligPow),0)];
	float ligDecay = gradationTexture.SampleLevel(Sampler,float2(saturate(ligPow),0.f),0);
	ligDecay *= floor(ligPow);

	float colorPow = length(light.color.xyz);
	//float spGradation = gradationTexture[uint2(saturate(rad-0.001f),1)];	//グラデーションマップ(明かり用)
	float spGradation = gradationTexture.SampleLevel(Sampler,float2(saturate(rad-0.001f),1.f),0);	//グラデーションマップ(明かり用)

	float spe = saturate(spGradation);
	float3 lig = (light.color.xyz*max(spe-ligDecay,0.f));
	foundation = lig*k;

	//反射
	float3 e2w;// = worldPos - eyepos;
	e2w = normalize(posInView);
	float3 w2pRef =  -dir + 2.0f * dot(dir,normal) * normal;
	float refDote2w = dot(w2pRef,e2w);

	float sp = pow(saturate(refDote2w),5.f);

	//spGradation = gradationTexture[uint2(saturate(sp),0)];	//グラデーションマップ(明かり用)
	spGradation = gradationTexture.SampleLevel(Sampler,float2(saturate(sp),0.f),0);	//グラデーションマップ(明かり用)

	lig = light.color.xyz*saturate((spGradation-ligDecay)*spe);
	foundation += lig*specular*k;

	return foundation;
}


[numthreads(TILE_WIDTH, TILE_WIDTH, 1)]
void CSMain_PointLightCulling(uint3 groupId:SV_GroupID, uint3 dispatchThreadId:SV_DispatchThreadID, uint3 GroupThreadId:SV_GroupThreadID)
{
	
    uint GroupIndex = GroupThreadId.y * TILE_WIDTH + GroupThreadId.x;
    //共有メモリを初期化する。
	//[branch]
    //if(GroupIndex == 0)
    {
		sTileNumLights = 0;
		sNumPerSamplePixels = 0;
		sMinZ = 0x7F7FFFFF;		// floatの最大値
		sMaxZ = 0;
	}

    float3 posInView = ComputePositionInCamera(dispatchThreadId.xy);
	// ここで同期を取る
    GroupMemoryBarrierWithGroupSync();
    
    // タイルの最大・最小深度を求める
	// この処理は並列するスレッド全てで排他的に処理される
	InterlockedMin( sMinZ, asuint(posInView.z) );
	InterlockedMax( sMaxZ, asuint(posInView.z) );
	
	// ここで同期を取ることでタイルの最大・最小深度を正しいものにする
    GroupMemoryBarrierWithGroupSync();
    
    // タイルの錘台を求める
	float4 frustumPlanes[6];
	GetTileFrustumPlane( frustumPlanes, groupId );

	// タイルとポイントライトの衝突判定
	[fastopt]
	for (uint lightIndex = GroupIndex; lightIndex < PLcount; lightIndex += TILE_SIZE)
	{
		SPointLight light = PntLights[lightIndex];

		// タイルとの判定
		bool inFrustum = true;
		[unroll(6)]
		for (uint i = 0; i < 6; ++i)
		{
			float4 lp = float4(light.posInView, 1.0f);
			float d = dot( frustumPlanes[i], lp );
			inFrustum = inFrustum && (d >= -light.radius);
		}

		// タイルと衝突している場合
		[branch]
		if (inFrustum)
		{
			uint listIndex;
			InterlockedAdd( sTileNumLights, 1, listIndex );
			sTileLightIndices[listIndex] = lightIndex;
		}
	}

	// ここで同期を取ると、sTileLightIndicesにタイルと衝突しているライトのインデックスが積まれている
    GroupMemoryBarrierWithGroupSync();
    
    float3 normal = normalTexture[dispatchThreadId.xy].xyz;
    //カメラ座標系でライティングの計算を行うので法線を回す回す。
    normal = mul(mtxViewRot, float4(normal, 1.0f)).xyz;

	float specular = specularTexture[dispatchThreadId.xy].x;

	//こっからライティング
	float3 foundation = 1.f;
	[loop]
	for(int i = 0;i<sTileNumLights;i++)
	{
		foundation += PointLightProcess(sTileLightIndices[i],posInView,normal,specular);
		//foundation += PntLights[sTileLightIndices[i]].color;
#if 0
		uint lightIndex = sTileLightIndices[i];
		SPointLight light = PntLights[lightIndex];
		float3 lightDir = posInView - light.pos;
		float len = length(lightDir);
		lightDir = normalize(lightDir);	//正規化。
		float3 pointLightColor = saturate(-dot(normal, lightDir)) * light.color.xyz;
		//減衰を計算する。
		float	litRate = len / light.radius.x;
		float	attn = max(1.0 - litRate * litRate, 0.0);
		pointLightColor *= attn;
		foundation += pointLightColor;
#endif
	}
	float3 diffuseColor = diffuseTexture[dispatchThreadId.xy];
	
	rwTexture[dispatchThreadId.xy] *= float4(foundation,1.f);
	return;
}