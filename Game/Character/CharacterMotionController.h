#pragma once
#include "CharacterMoveMotion.h"
#include "CharacterRotateMotion.h"

class Character;

class CharacterMotionController:public ue::GameObject
{
public:
	CharacterMotionController() {};
	virtual ~CharacterMotionController() {};

	/// <summary>
	/// 開放。
	/// </summary>
	virtual void Release() override;
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual void OnDestroy() override;

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="chara"></param>
	/// <param name="animClip"></param>
	void Init(Character* chara,ue::AnimationClip* animClip);
	/// <summary>
	/// 移動用の初期化。
	/// </summary>
	/// <param name="footL"></param>
	/// <param name="footR"></param>
	void InitMove(ue::Bone* footL, ue::Bone* footR,float startUPMax = 0.0f, float upSpeed=0.0f);
	/// <summary>
	/// 回転用の初期化。
	/// </summary>
	/// <param name="rotateMode"></param>
	/// <param name="speedDeg"></param>
	void InitRotate(CharacterRotateMotion::RotateMode rotateMode, float speedDeg);

	//スタート
	bool Start();
	//更新。
	void Update();

	/// <summary>
	/// 移動する。
	/// </summary>
	/// <param name="playNum"></param>
	/// <param name="speed"></param>
	/// <param name="actionMode"></param>
	void PlayMove(unsigned int playNum,float speed,ActionMode actionMode);
	/// <summary>
	/// 回転する。
	/// </summary>
	/// <param name="dir"></param>
	/// <param name="playNum"></param>
	/// <param name="isPlayAnim"></param>
	void PlayRotate(const ue::CVector3& dir, unsigned int playNum,bool isPlayAnim=false);

	void PlayIdol();
private:
	Character* m_chara = nullptr;			//キャラクター

	CharacterMoveMotion m_moveMotion;		//移動。
	CharacterRotateMotion m_rotateMotion;	//回転。

	enum PlayMotion
	{
		PM_Idol,
		PM_Move,
		PM_Rotate,
	};
	PlayMotion m_playMotion = PM_Idol;

	unsigned int m_idolNum = 0;

	int m_initCount = 0;					//初期化した?
	bool m_isPlatAnimRotate = false;		//回転時にアニメーションを再生している?

	
};