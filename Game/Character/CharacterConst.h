#pragma once 

//namespace character

/// <summary>
/// 動きのステート
/// </summary>
enum ActionMode
{
	AM_None,
	AM_Move,
	AM_Rotate,
};

/// <summary>
/// ボーンの部位.
/// </summary>
enum BoneKind
{
	BK_None,
	BK_FootL,
	BK_FootR,
	BK_HandL,
	BK_HandR,
	BK_WaistL,
	BK_WaistR,
};