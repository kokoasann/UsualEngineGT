#pragma once

#include "graphics/ConstantBuffer.h"
#include "graphics/StructuredBuffer.h"
#include "LightStruct.h"


namespace UsualEngine
{
#define MAX_DIRLIGHT 8
	class LightBase;
	class LightDirection;

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

		void InitDirectionStructuredBuffet();

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

		//描画終了
		void EndRender();

		/// <summary>
		/// 0番目のディレクションライトを返す
		/// </summary>
		/// <returns></returns>
		LightDirection* GetMainLightDirection()
		{
			if (mCDirLight.size() > 0)
			{
				return mCDirLight[0];
			}
			else
			{
				return nullptr;
			}
		}
		//ライトのパラメータの構造体
		struct LightParam
		{
			CVector3 eyePos = CVector3::Zero();		//メインカメラの位置
			int DLCount = 0;						//ディレクションライトの数
			CVector4 screen;						//スクリーンのサイズ
		};
	private:
		LightParam mLightParam;						//ライトの情報
		ConstantBuffer mLightParamCB;				//ライトパラメータの定数バッファ
		std::vector<LightDirection*> mCDirLight;	//ディレクションライトのリスト
		SDirectionLight mSDirLights[MAX_DIRLIGHT];	//ディレクションライト構造体のリスト
		StructuredBuffer mDirLightSB;				//ディレクションライトのStructuredBuffer
	};
}