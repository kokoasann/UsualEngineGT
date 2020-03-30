/*!
 * @brief	アニメーション
 */

#pragma once

#include "graphics/animation/AnimationClip.h"
#include "graphics/animation/AnimationPlayController.h"

#include "Physics/SphereCollider.h"


#include "IK.h"

namespace UsualEngine
{
	class Skeleton;
	class SkinModel;
	/*!
	* @brief	アニメーションクラス。
	*/
	class Animation {
	public:
		using EventListener = std::function<void(const std::string& clipname,const std::string& eventname)>;

		Animation();
		~Animation();
		void Release();
		/*!
			*@brief	初期化。
			*@param[in]	skinModel		アニメーションさせるスキンモデル。
			*@param[in]	animeClipList	アニメーションクリップの配列。
			*@param[in]	numAnimClip		アニメーションクリップの数。
			*/
		void Init(SkinModel& skinModel, AnimationClip animClipList[], int numAnimClip);


		/*!
		*@brief	アニメーションの再生。
		*@param[in]	clipNo	アニメーションクリップの番号。Init関数に渡したanimClipListの並びとなる。
		*@param[in]	interpolateTime		補完時間(単位：秒)
		*/
		void Play(int clipNo, float interpolateTime = 0.0f,float startTime = 0.0f)
		{
			PlayCommon(m_animationClips[clipNo], interpolateTime,startTime);
		}
		/*!
		* @brief	アニメーションの再生中？
		*/
		bool IsPlaying() const
		{
			int lastIndex = GetLastAnimationControllerIndex();
			return m_animationPlayController[lastIndex].IsPlaying();
		}

		/*!
		* @brief	アニメーションを進める。
		*@details
		* エンジン内部から呼ばれます。
		* ユーザーは使用しないでください。
		*@param[in]	deltaTime		アニメーションを進める時間(単位：秒)。
		*/
		void Update(float deltaTime);

		AnimationPlayController* GetAnimationPlayController(int& num)
		{
			num = ANIMATION_PLAY_CONTROLLER_NUM;
			return m_animationPlayController;
		}

		void SetWorldMatrix(CMatrix mat)
		{
			m_worldMatrix = mat;
		}

		void AddEventListener(EventListener ev)
		{
			m_eventListener.push_back(ev);
		}
		void EventListen(const std::string& clipname,const std::string& eventname)
		{
			for (auto& el : m_eventListener)
			{
				el(clipname, eventname);
			}
		}

		/// <summary>
		/// IKをセッティングします。
		/// </summary>
		/// <param name="effector">IKの</param>
		/// <param name="end"></param>
		/// <param name="radius"></param>
		/// <param name="modelPos"></param>
		IK* SetingIK(Bone* effector, Bone* end,float radius,bool isCreateRigitBody,const CVector3& modelPos)
		{
			IK* ik = new IK(effector, end, radius, isCreateRigitBody, modelPos);
			m_ik.push_back(ik);
			return ik;
		}
		IK* GetIK(int num)
		{
			return m_ik[num];
		}

		void SetIKMode(int mode, Bone* bone = nullptr);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="bone"></param>
		const CVector3& GetIKTarget(Bone* bone) const
		{
			for (auto ik : m_ik)
			{
				if (bone == ik->GetEffectorBone())
				{
					return ik->GetTarget();
				}
			}
		}
		
		/// <summary>
		/// アニメーションの保持するIKの摩擦率をセットする
		/// </summary>
		/// <param name="f">1~0がいいと思う。</param>
		void SetIKRub(float f, Bone* bone = nullptr);
		
		void SetIKOffset(const CVector3& ofs, Bone* bone = nullptr);
		
		void SetIKSpeed(float speed, Bone* bone = nullptr);

		void SetIKActive(bool b, Bone* bone = nullptr);
		
		void SetIKNextTarget(const CVector3& tar, Bone* bone);

		void SetIKGravity(float g, Bone* bone = nullptr);

		void QueryIK(std::function<void(IK*)> ikfunc);
		/// <summary>
		/// 
		/// </summary>
		void UpdateIKTarget();
		/// <summary>
		/// 
		/// </summary>
		void UpdateIK();
		
		int GetFrameNum() const
		{
			return m_animationPlayController[GetLastAnimationControllerIndex()].GetFrameNum();
		}

		int GetCurrentFrameNo() const
		{
			return m_animationPlayController[GetLastAnimationControllerIndex()].GetCurrentKeyFrameNo();
		}
	private:
		void PlayCommon(AnimationClip* nextClip, float interpolateTime, float startTime)
		{
			int index = GetLastAnimationControllerIndex();
			if (m_animationPlayController[index].GetAnimClip() == nextClip) {
				return;
			}
			if (interpolateTime == 0.0f) {
				//補完なし。
				m_numAnimationPlayController = 1;
			}
			else {
				//補完あり。
				m_numAnimationPlayController++;
			}
			index = GetLastAnimationControllerIndex();
			m_animationPlayController[index].ChangeAnimationClip(nextClip);
			m_animationPlayController[index].SetInterpolateTime(interpolateTime);
			m_animationPlayController[index].SetTime(startTime);
			m_interpolateTime = 0.0f;
			m_interpolateTimeEnd = interpolateTime;
		}
		/*!
			* @brief	ローカルポーズの更新。
			*/
		void UpdateLocalPose(float deltaTime);
		/*!
			* @brief	グローバルポーズの更新。
			*/
		void UpdateGlobalPose();
	private:

		/*!
			*@brief	最終ポーズになるアニメーションのリングバッファ上でのインデックスを取得。
			*/
		int GetLastAnimationControllerIndex() const
		{
			return GetAnimationControllerIndex(m_startAnimationPlayController, m_numAnimationPlayController - 1);
		}
		/*!
		*@brief	アニメーションコントローラのリングバッファ上でのインデックスを取得。
		*@param[in]	startIndex		開始インデックス。
		*@param[in]	localIndex		ローカルインデックス。
		*/
		int GetAnimationControllerIndex(int startIndex, int localIndex) const
		{
			return (startIndex + localIndex) % ANIMATION_PLAY_CONTROLLER_NUM;
		}
	private:
		static const int ANIMATION_PLAY_CONTROLLER_NUM = 32;			//!<アニメーションコントローラの数。
		std::vector<AnimationClip*>	m_animationClips;					//!<アニメーションクリップの配列。
		Skeleton* m_skeleton = nullptr;	//!<アニメーションを適用するスケルトン。
		AnimationPlayController	m_animationPlayController[ANIMATION_PLAY_CONTROLLER_NUM];	//!<アニメーションコントローラ。リングバッファ。
		int m_numAnimationPlayController = 0;		//!<現在使用中のアニメーション再生コントローラの数。
		int m_startAnimationPlayController = 0;		//!<アニメーションコントローラの開始インデックス。
		float m_interpolateTime = 0.0f;
		float m_interpolateTimeEnd = 0.0f;
		bool m_isInterpolate = false;				//!<補間中？

		CMatrix m_worldMatrix;

		std::vector<EventListener> m_eventListener;

		std::vector<IK*> m_ik;
	};
}