#pragma once


namespace UsualEngine
{
	class SkinModelRender :public GameObject
	{
	public:
		
		void Update() override;
		void Render() override;
		
		void Init(const wchar_t* path, AnimationClip* anims = nullptr, int animCount = 0, EnFbxUpAxis axis = enFbxUpAxisZ);
		
		void SetPos(CVector3 pos)
		{
			m_position = pos;
		}
		
		CVector3 GetPos()
		{
			return m_position;
		}
		void SetRot(CQuaternion rot)
		{
			m_rotation = rot;
		}
		CQuaternion GetRot()
		{
			return m_rotation;
		}
		
		void SetSca(CVector3 sca)
		{
			m_scale = sca;
		}
		CVector3 GetSca()
		{
			return m_scale;
		}

		void SetIsShadowCaster(bool b)
		{
			m_skinModel.SetIsShadowCaster(b);
		}
		bool IsShadowCaster()
		{
			return m_skinModel.IsShadowCaster();
		}

		/// <summary>
		/// ƒXƒLƒ“ƒ‚ƒfƒ‹‚ð•Ô‚·
		/// </summary>
		/// <returns></returns>
		SkinModel& GetSkinModel()
		{
			return m_skinModel;
		}
	private:
		SkinModel m_skinModel;	//siknModel
		Animation m_animation;	//Animation

		CVector3 m_position = CVector3::Zero();
		CQuaternion m_rotation = CQuaternion::Identity();
		CVector3 m_scale = CVector3::One();
	};
}