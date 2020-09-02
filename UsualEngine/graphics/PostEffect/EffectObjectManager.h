#pragma once


#include "EffectObject.h"

namespace UsualEngine
{
	class EffectObject;

	/// <summary>
	/// �V���O���g��
	/// �G�t�F�N�g�I�u�W�F�N�g���܂Ƃ߂Ă��B
	/// </summary>
	class EffectObjectManager
	{
	private:
		EffectObjectManager() {};
		
	public:
		~EffectObjectManager() {};
		/// <summary>
		/// �����̃C���X�^���X���Q�b�g
		/// </summary>
		/// <returns></returns>
		static EffectObjectManager& Get()
		{
			static EffectObjectManager eom;
			return eom;
		}
		/// <summary>
		/// �G�t�F�N�g�̒ǉ��B
		/// </summary>
		/// <param name="eff"></param>
		void AddEffect(EffectObject* eff)
		{
			m_effectList.push_back(eff);
		}
		/// <summary>
		/// �G�t�F�N�g�̍폜�B
		/// </summary>
		/// <param name="eff"></param>
		void RemoveEffect(EffectObject* eff)
		{
			auto ite = std::find(m_effectList.begin(), m_effectList.end(), eff);
			m_effectList.erase(ite);
		}
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="pe"></param>
		void Render(PostEffect* pe);
	private:
		std::vector<EffectObject*> m_effectList;	//�G�t�F�N�g�����̃��X�g
	};


	/// <summary>
	/// �G�t�F�N�g�`��p�I�u�W�F�N�g
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
		/// �`��B
		/// </summary>
		/// <param name="pe"></param>
		virtual void Render(PostEffect* pe) = 0;

		/// <summary>
		/// �Đ�����
		/// </summary>
		void Start()
		{
			m_isPlay = true;
		}
		/// <summary>
		/// �Đ����Ȃ�
		/// </summary>
		void Stop()
		{
			m_isPlay = false;
		}
		/// <summary>
		/// Start Stop�̐؂�ւ�
		/// </summary>
		void Toggle()
		{
			m_isPlay ^= true;
		}

		/// <summary>
		/// �Đ����Ă�?
		/// </summary>
		/// <returns></returns>
		bool IsPlay() const
		{
			return m_isPlay;
		}
	private:
		bool m_isPlay = true;	//�Đ���?
	};
}