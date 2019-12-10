#pragma once

namespace UsualEngine
{
	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();

		void Revive()
		{
			mIsStart = false;
			mIsDead = false;
			mIsActive = true;
			mName = 0;
			mPrio = 0;
			mIsTrashTake = false;
		}

		//デストラクラ
		virtual void OnDestroy() {};
		//早めのコンストラクタ
		virtual void Awake() {};
		//スタート関数　戻り値がfalseの場合は次のフレームでもっかい呼ばれる
		virtual bool Start() { return true; };
		//スタート関数でtrueが返ってきた次のフレームから毎フレーム呼ばれるようになる。
		virtual void Update() {};

		virtual void Render() {};
		virtual void PostRender() {};

		void WrapOnDestroy(){}
		void WrapStart();
		void WrapUpdate();
		void WrapRender();
		void WrapPostRender();

		/*
		ゲーム側では使わない
		FindGOとかで識別するための名前をセットする
		*/
		void SetName(int name)
		{
			mName = name;
		}
		/*
		ゲーム側では使わない
		処理の優先度。数が低いほど先に呼ばれる。
		*/
		void SetPrio(int prio)
		{
			mPrio = prio;
		}
		//優先度を返す
		int GetPrio()
		{
			return mPrio;
		}

		//スタート関数でtrueが返ってきたか。
		bool isStart()
		{
			return mIsStart;
		}
		//スタートした
		void START()
		{
			mIsStart = true;
		}

		//死んでる？
		bool isDead()
		{
			return mIsDead;
		}
		//死んだ
		void DEAD()
		{
			mIsDead = true;
		}

		//アクティブ？
		bool IsActive()
		{
			return mIsActive;
		}
		//アクティブフラグをセットする
		void SetActive(bool b)
		{
			mIsActive = b;
		}

		//ゴミ拾いをする
		void TrashTaking()
		{
			mIsTrashTake = true;
		}
		//ゴミ拾いする？
		bool IsTrashTake()
		{
			return mIsTrashTake;
		}

		

	private:
		bool mIsStart = false;			//スタート関数呼ばれた？
		bool mIsDead = false;			//死んだ？
		bool mIsActive = true;			//動ける？

		int mName = 0;					//名前(hash値)
		int mPrio = 0;						//プライオリティ

		bool mIsTrashTake = true;	//ゴミ拾う？
	protected:
	};

}

