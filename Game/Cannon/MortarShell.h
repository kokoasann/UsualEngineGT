#pragma once

class MortarShell:public ue::GameObject
{
public:
	MortarShell(){}
	~MortarShell();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;

private:
	ue::SkinModelRender* m_model = nullptr;
	ue::CVector3 m_pos = ue::CVector3::Zero();
	ue::CQuaternion m_rot = ue::CQuaternion::Identity();
};