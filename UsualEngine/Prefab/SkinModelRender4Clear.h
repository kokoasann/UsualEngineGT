#pragma once


namespace UsualEngine
{
	class SkinModelRender4Clear :public GameObject
	{
	public:

		virtual void Release() override;
		virtual void OnDestroy() override;

		void Init(const wchar_t* path, EnFbxUpAxis axis);

		void Update() override;
	private:
		SkinModel m_skinModel;
		
	};
}