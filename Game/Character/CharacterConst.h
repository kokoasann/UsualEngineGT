#pragma once 

//namespace character

#define CHARA_IDOL_NUM 0

/// <summary>
/// �����̃X�e�[�g
/// </summary>
enum ActionMode
{
	AM_None,
	AM_Move,
	AM_Rotate,
};

/// <summary>
/// �{�[���̕���.
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