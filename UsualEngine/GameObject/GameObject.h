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
			mIsStart = false;
			mIsDead = false;
			mIsActive = true;
			mName = 0;
			mPrio = 0;
			mIsTrashTake = false;
		}

		//�f�X�g���N��
		virtual void OnDestroy() {};
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
			mName = name;
		}
		/*
		�Q�[�����ł͎g��Ȃ�
		�����̗D��x�B�����Ⴂ�قǐ�ɌĂ΂��B
		*/
		void SetPrio(int prio)
		{
			mPrio = prio;
		}
		//�D��x��Ԃ�
		int GetPrio()
		{
			return mPrio;
		}

		//�X�^�[�g�֐���true���Ԃ��Ă������B
		bool isStart()
		{
			return mIsStart;
		}
		//�X�^�[�g����
		void START()
		{
			mIsStart = true;
		}

		//����ł�H
		bool isDead()
		{
			return mIsDead;
		}
		//����
		void DEAD()
		{
			mIsDead = true;
		}

		//�A�N�e�B�u�H
		bool IsActive()
		{
			return mIsActive;
		}
		//�A�N�e�B�u�t���O���Z�b�g����
		void SetActive(bool b)
		{
			mIsActive = b;
		}

		//�S�~�E��������
		void TrashTaking()
		{
			mIsTrashTake = true;
		}
		//�S�~�E������H
		bool IsTrashTake()
		{
			return mIsTrashTake;
		}

		

	private:
		bool mIsStart = false;			//�X�^�[�g�֐��Ă΂ꂽ�H
		bool mIsDead = false;			//���񂾁H
		bool mIsActive = true;			//������H

		int mName = 0;					//���O(hash�l)
		int mPrio = 0;						//�v���C�I���e�B

		bool mIsTrashTake = true;	//�S�~�E���H
	protected:
	};

}

