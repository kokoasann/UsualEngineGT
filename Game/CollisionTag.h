#pragma once

enum CollisionUserIndex
{
	CUI_Cannon = enCollisionAttr_User,
	CUI_MortarShell = enCollisionAttr_User << 1,
	CUI_Monster = enCollisionAttr_User << 2,
};