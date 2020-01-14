#pragma once

#include "physics/PhysicsStaticObject.h"

class Game :public ue::GameObject
{
public:
	Game();
	/// <summary>
	/// äJï˙ÅB
	/// </summary>
	void Release() override{}
	/// <summary>
	/// DeleteGOÇ≥ÇÍÇΩèuä‘åƒÇŒÇÍÇÈ
	/// </summary>
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

	ue::Bone* Lfoot=0;
	ue::Bone* Rfoot=0;
	ue::Bone* Rwaist=0;
	bool m_isleftON = false;
	bool m_isrightON = false;
	std::array<ue::CVector3, 3> m_movedata;
	int count = 0;
	ue::CharacterController cc;
	float modeloffset = 30.f;

	ue::PhysicsStaticObject pso;
	ue::PhysicsStaticObject pso2;

	ue::LightDirection* light = 0;
};