#pragma once

namespace UsualEngine
{
	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();

		void Revive()
		{
			m_isStartGO = false;
			m_isDeadGO = false;
			m_isActiveGO = true;
			m_nameGO = 0;
			m_prioGO = 0;
			m_isTrashTakeGO = true;
		}

		virtual void Release() = 0;

		//�f�X�g���N��
		virtual void OnDestroy() =0;
		//���߂̃R���X�g���N�^
		virtual void Awake() {};
		//�X�^�[�g�֐��@�߂�l��false�̏ꍇ�͎��̃t���[���ł��������Ă΂��
		virtual bool Start() { return true; };
		//�X�^�[�g�֐���true���Ԃ��Ă������̃t���[�����疈�t���[���Ă΂��悤�ɂȂ�B
		virtual void Update() {};

		virtual void Render() {};
		virtual void PostRender() {};

		void WrapOnDestroy(){}
		void WrapStart();
		void WrapUpdate();
		void WrapRender();
		void WrapPostRender();

		/*
		�Q�[�����ł͎g��Ȃ�
		FindGO�Ƃ��Ŏ��ʂ��邽�߂̖��O���Z�b�g����
		*/
		void SetName(int name)
		{
			m_nameGO = name;
		}
		/*
		�Q�[�����ł͎g��Ȃ�
		�����̗D��x�B�����Ⴂ�قǐ�ɌĂ΂��B
		*/
		void SetPrio(int prio)
		{
			m_prioGO = prio;
		}
		//�D��x��Ԃ�
		int GetPrio()
		{
			return m_prioGO;
		}

		//�X�^�[�g�֐���true���Ԃ��Ă������B
		bool IsStart()
		{
			return m_isStartGO;
		}
		//�X�^�[�g����
		void DoneStart()
		{
			m_isStartGO = true;
		}

		//����ł�H
		bool IsDead()
		{
			return m_isDeadGO;
		}
		//����
		void DoneDead()
		{
			m_isDeadGO = true;
		}

		//�A�N�e�B�u�H
		bool IsActive()
		{
			return m_isActiveGO;
		}
		//�A�N�e�B�u�t���O���Z�b�g����
		void SetActive(bool b)
		{
			m_isActiveGO = b;
		}

		//�S�~�E��������
		void TrashTaking()
		{
			m_isTrashTakeGO = true;
		}
		//�S�~�E������H
		bool IsTrashTake()
		{
			return m_isTrashTakeGO;
		}

		

	private:
		bool m_isStartGO = false;			//�X�^�[�g�֐��Ă΂ꂽ�H
		bool m_isDeadGO = false;			//���񂾁H
		bool m_isActiveGO = true;			//������H

		int m_nameGO = 0;					//���O(hash�l)
		int m_prioGO = 0;						//�v���C�I���e�B

		bool m_isTrashTakeGO = true;	//�S�~�E���H
	protected:
	};

}

