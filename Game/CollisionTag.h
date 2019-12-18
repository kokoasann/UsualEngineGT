#pragma once

enum CollisionUserIndex
{
	CUI_Cannon = ue::enCollisionAttr_User,
	CUI_MortarShell = ue::enCollisionAttr_User << 1,
	CUI_Monster = ue::enCollisionAttr_User << 2,
	CUI_Monster_Head = ue::enCollisionAttr_User << 3,
};

