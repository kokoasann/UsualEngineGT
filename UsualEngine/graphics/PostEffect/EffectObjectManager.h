#pragma once


#include "EffectObject.h"

namespace UsualEngine
{
	class EffectObject;

	/// <summary>
	/// シングルトン
	/// エフェクトオブジェクトをまとめてるやつ。
	/// </summary>
	class EffectObjectManager
	{
	private:
		EffectObjectManager() {};
		
	public:
		~EffectObjectManager() {};
		/// <summary>
		/// こいつのインスタンスをゲット
		/// </summary>
		/// <returns></returns>
		static EffectObjectManager& Get()
		{
			static EffectObjectManager eom;
			return eom;
		}
		/// <summary>
		/// エフェクトの追加。
		/// </summary>
		/// <param name="eff"></param>
		void AddEffect(EffectObject* eff)
		{
			m_effectList.push_back(eff);
		}
		/// <summary>
		/// エフェクトの削除。
		/// </summary>
		/// <param name="eff"></param>
		void RemoveEffect(EffectObject* eff)
		{
			auto ite = std::find(m_effectList.begin(), m_effectList.end(), eff);
			m_effectList.erase(ite);
		}
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="pe"></param>
		void Render(PostEffect* pe);
	private:
		std::vector<EffectObject*> m_effectList;	//エフェクトたちのリスト
	};


	/// <summary>
	/// エフェクト描画用オブジェクト
	/// </summary>
	class EffectObject
	{
	public:
		EffectObject()
		{
			EffectObjectManager::Get().AddEffect(this);
		}
		virtual ~EffectObject()
		{
			EffectObjectManager::Get().AddEffect(this);
		}

		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="pe"></param>
		virtual void Render(PostEffect* pe) = 0;

		/// <summary>
		/// 再生する
		/// </summary>
		void Start()
		{
			m_isPlay = true;
		}
		/// <summary>
		/// 再生しない
		/// </summary>
		void Stop()
		{
			m_isPlay = false;
		}
		/// <summary>
		/// Start Stopの切り替え
		/// </summary>
		void Toggle()
		{
			m_isPlay ^= true;
		}

		/// <summary>
		/// 再生してる?
		/// </summary>
		/// <returns></returns>
		bool IsPlay() const
		{
			return m_isPlay;
		}
	private:
		bool m_isPlay = true;	//再生中?
	};
}