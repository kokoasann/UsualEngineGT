#pragma once


namespace UsualEngine
{
	class SpriteRender :public GameObject
	{
	public:
		SpriteRender() {};
		~SpriteRender() {};

		void OnDestroy();

		void Init(const wchar_t* path, CVector2 imageSize);

		void Update();

		void PostRender() override;

		/*
			mPosition�ɃZ�b�g����
		*/
		void SetPos(CVector3 pos)
		{
			mPosition = pos;
		}
		
		/*
			mRotation�ɃZ�b�g����
		*/
		void SetRot(CQuaternion rot)
		{
			mRotation = rot;
		}

		/*
			mScale�ɃZ�b�g����
		*/
		void SetSca(CVector3 sca)
		{
			mScale = sca;
		}

		/*
			mPosition���Q�b�g����
		*/
		CVector3 GetPos()
		{
			return mPosition;
		}

		/*
			mRotation���Q�b�g����
		*/
		CQuaternion GetRot()
		{
			return mRotation;
		}

		/*
			mScale���Q�b�g����
		*/
		CVector3 GetSca()
		{
			return mScale;
		}
	private:
		Sprite mSprite;											//�X�v���C�g
		CVector3 mPosition = CVector3::Zero();					//�|�W�V����
		CQuaternion mRotation = CQuaternion::Identity();		//���[�e�[�V����
		CVector3 mScale = CVector3::One();						//�X�P�[��
		CVector2 mPivot = {0.5f,0.5f};							//�s�{�b�g

	};
}