#pragma once

namespace ue = UsualEngine;

enum CollisionUserIndex
{
	CUI_Cannon = ue::enCollisionAttr_User,
	CUI_MortarShell = ue::enCollisionAttr_User << 1,
	CUI_Monster = ue::enCollisionAttr_User << 2,
};

enum MonsterTag
{
	MT_Base = CUI_Monster,
	MT_Head = MT_Base << 1,
};