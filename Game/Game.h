#pragma once


class Game :public ue::GameObject
{
public:
	Game();

	void OnDestroy() override;
	bool Start() override;
	void Update() override;
private:
	ue::AnimationClip animclip[5];
	ue::SkinModelRender* p1 = 0;
	ue::SkinModelRender* p2 = 0;
	ue::SkinModelRender* p3 = 0;
	ue::SMR4Ground* ground = 0;
	ue::CVector3 campos;
	ue::CQuaternion rot = ue::CQuaternion::Identity();
	ue::Camera* cam = 0;
	ue::CQuaternion camrot=ue::CQuaternion::Identity();

	ue::LightDirection* light = 0;
};