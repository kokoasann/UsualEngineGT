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
			mPositionにセットする
		*/
		void SetPos(CVector3 pos)
		{
			mPosition = pos;
		}
		
		/*
			mRotationにセットする
		*/
		void SetRot(CQuaternion rot)
		{
			mRotation = rot;
		}

		/*
			mScaleにセットする
		*/
		void SetSca(CVector3 sca)
		{
			mScale = sca;
		}

		/*
			mPositionをゲットする
		*/
		CVector3 GetPos()
		{
			return mPosition;
		}

		/*
			mRotationをゲットする
		*/
		CQuaternion GetRot()
		{
			return mRotation;
		}

		/*
			mScaleをゲットする
		*/
		CVector3 GetSca()
		{
			return mScale;
		}
	private:
		Sprite mSprite;											//スプライト
		CVector3 mPosition = CVector3::Zero();					//ポジション
		CQuaternion mRotation = CQuaternion::Identity();		//ローテーション
		CVector3 mScale = CVector3::One();						//スケール
		CVector2 mPivot = {0.5f,0.5f};							//ピボット

	};
}