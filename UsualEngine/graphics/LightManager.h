#pragma once

#include "graphics/ConstantBuffer.h"
#include "graphics/StructuredBuffer.h"
#include "graphics/Shader.h"
#include "LightStruct.h"


namespace UsualEngine
{
#define MAX_DIRLIGHT 8
#define MAX_PNTLIGHT 32
	class LightBase;
	class LightDirection;
	class LightPoint;

	/*
	ライトを管理するやつ
	*/
	class LightManager
	{
	friend class GraphicsEngine;
	private:
		LightManager();
		~LightManager();
	public:
		////こいつのインスタンスをゲット！！
		//static LightManager* Get()
		//{
		//	static LightManager ins;
		//	
		//	return &ins;
		//}

		//初期化
		void Init();

		void InitDirectionStructuredBuffer();

		void InitPointStructuredBuffer();

		/*
		ライト追加
		*/
		void AddLight(LightBase* light);

		/*
		ライト除去
		*/
		void RmvLight(LightBase* light);

		//更新
		void Update();

		//描画
		void Render();

		/// <summary>
		/// 
		/// </summary>
		void RenderPointLight();

		//描画終了
		void EndRender();

		/// <summary>
		/// 0番目のディレクションライトを返す
		/// </summary>
		/// <returns></returns>
		LightDirection* GetMainLightDirection()
		{
			if (m_cDirLight.size() > 0)
			{
				return m_cDirLight[0];
			}
			else
			{
				return nullptr;
			}
		}
		//ライトのパラメータの構造体
		struct LightParam
		{
			CVector4 screen;						//スクリーンのサイズ
			CVector3 eyePos = CVector3::Zero();		//メインカメラの位置
			int DLCount = 0;						//ディレクションライトの数
			int PLCount = 0;						//ポイントライトの数。
		};
	private:
		LightParam m_lightParam;						//ライトの情報
		ConstantBuffer m_lightParamCB;				//ライトパラメータの定数バッファ
		std::vector<LightDirection*> m_cDirLight;	//ディレクションライトのリスト
		SDirectionLight m_sDirLights[MAX_DIRLIGHT];	//ディレクションライト構造体のリスト
		StructuredBuffer m_dirLightSB;				//ディレクションライトのStructuredBuffer
		std::vector<LightPoint*> m_cPointLight;
		SPointLight m_sPntLights[MAX_PNTLIGHT];
		StructuredBuffer m_pntLightSB;

		struct CameraParam
		{
			CMatrix projMat;
			CMatrix projInvMat;
			CMatrix viewRotMat;
			float camNear;
			float camFar;
			float screenWidth;
			float screenHeight;
		};
		ConstantBuffer m_cameraParamCB;
		Shader m_csPointLightCulling;
	};
}