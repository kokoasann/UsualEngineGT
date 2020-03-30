/*!
 * @brief	�A�j���[�V����
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
	* @brief	�A�j���[�V�����N���X�B
	*/
	class Animation {
	public:
		using EventListener = std::function<void(const std::string& clipname,const std::string& eventname)>;

		Animation();
		~Animation();
		void Release();
		/*!
			*@brief	�������B
			*@param[in]	skinModel		�A�j���[�V����������X�L�����f���B
			*@param[in]	animeClipList	�A�j���[�V�����N���b�v�̔z��B
			*@param[in]	numAnimClip		�A�j���[�V�����N���b�v�̐��B
			*/
		void Init(SkinModel& skinModel, AnimationClip animClipList[], int numAnimClip);


		/*!
		*@brief	�A�j���[�V�����̍Đ��B
		*@param[in]	clipNo	�A�j���[�V�����N���b�v�̔ԍ��BInit�֐��ɓn����animClipList�̕��тƂȂ�B
		*@param[in]	interpolateTime		�⊮����(�P�ʁF�b)
		*/
		void Play(int clipNo, float interpolateTime = 0.0f,float startTime = 0.0f)
		{
			PlayCommon(m_animationClips[clipNo], interpolateTime,startTime);
		}
		/*!
		* @brief	�A�j���[�V�����̍Đ����H
		*/
		bool IsPlaying() const
		{
			int lastIndex = GetLastAnimationControllerIndex();
			return m_animationPlayController[lastIndex].IsPlaying();
		}

		/*!
		* @brief	�A�j���[�V������i�߂�B
		*@details
		* �G���W����������Ă΂�܂��B
		* ���[�U�[�͎g�p���Ȃ��ł��������B
		*@param[in]	deltaTime		�A�j���[�V������i�߂鎞��(�P�ʁF�b)�B
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
		/// IK���Z�b�e�B���O���܂��B
		/// </summary>
		/// <param name="effector">IK��</param>
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
		/// �A�j���[�V�����̕ێ�����IK�̖��C�����Z�b�g����
		/// </summary>
		/// <param name="f">1~0�������Ǝv���B</param>
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
				//�⊮�Ȃ��B
				m_numAnimationPlayController = 1;
			}
			else {
				//�⊮����B
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
			* @brief	���[�J���|�[�Y�̍X�V�B
			*/
		void UpdateLocalPose(float deltaTime);
		/*!
			* @brief	�O���[�o���|�[�Y�̍X�V�B
			*/
		void UpdateGlobalPose();
	private:

		/*!
			*@brief	�ŏI�|�[�Y�ɂȂ�A�j���[�V�����̃����O�o�b�t�@��ł̃C���f�b�N�X���擾�B
			*/
		int GetLastAnimationControllerIndex() const
		{
			return GetAnimationControllerIndex(m_startAnimationPlayController, m_numAnimationPlayController - 1);
		}
		/*!
		*@brief	�A�j���[�V�����R���g���[���̃����O�o�b�t�@��ł̃C���f�b�N�X���擾�B
		*@param[in]	startIndex		�J�n�C���f�b�N�X�B
		*@param[in]	localIndex		���[�J���C���f�b�N�X�B
		*/
		int GetAnimationControllerIndex(int startIndex, int localIndex) const
		{
			return (startIndex + localIndex) % ANIMATION_PLAY_CONTROLLER_NUM;
		}
	private:
		static const int ANIMATION_PLAY_CONTROLLER_NUM = 32;			//!<�A�j���[�V�����R���g���[���̐��B
		std::vector<AnimationClip*>	m_animationClips;					//!<�A�j���[�V�����N���b�v�̔z��B
		Skeleton* m_skeleton = nullptr;	//!<�A�j���[�V������K�p����X�P���g���B
		AnimationPlayController	m_animationPlayController[ANIMATION_PLAY_CONTROLLER_NUM];	//!<�A�j���[�V�����R���g���[���B�����O�o�b�t�@�B
		int m_numAnimationPlayController = 0;		//!<���ݎg�p���̃A�j���[�V�����Đ��R���g���[���̐��B
		int m_startAnimationPlayController = 0;		//!<�A�j���[�V�����R���g���[���̊J�n�C���f�b�N�X�B
		float m_interpolateTime = 0.0f;
		float m_interpolateTimeEnd = 0.0f;
		bool m_isInterpolate = false;				//!<��Ԓ��H

		CMatrix m_worldMatrix;

		std::vector<EventListener> m_eventListener;

		std::vector<IK*> m_ik;
	};
}