#pragma once


namespace UsualEngine
{
	class SkinModelRender :public GameObject
	{
	public:
		void Update() override;
		void Render() override;

		void Init(const wchar_t* path, AnimationClip* anims = nullptr, int animCount = 0, EnFbxUpAxis axis = enFbxUpAxisZ);
	private:
		SkinModel mSkinModel;	//siknModel
		Animation mAnimation;	//Animation

		CVector3 mPosition = CVector3::Zero();
		CQuaternion mRotation = CQuaternion::Identity();
		CVector3 mScale = CVector3::One();
	};
}