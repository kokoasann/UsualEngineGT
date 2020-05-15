#pragma once

#include "physics/PhysicsStaticObject.h"

/// <summary>
/// テスト用
/// </summary>
class Game :public ue::GameObject
{
public:
	Game();
	/// <summary>
	/// 開放。
	/// </summary>
	void Release() override{}
	/// <summary>
	/// DeleteGOされた瞬間呼ばれる
	/// </summary>
	void OnDestroy() override;
	bool Start() override;
	void Update() override;

	void PostRender() override;
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
	ue::PhysicsStaticObject wpso[9];

	ue::LightDirection* light = 0;

	ue::Font m_font;
};