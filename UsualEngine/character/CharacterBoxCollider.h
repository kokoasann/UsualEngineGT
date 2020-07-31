#pragma once
#include "physics/BoxCollider.h"

namespace UsualEngine
{
	/// <summary>
	/// blenderで設定した場所にbox colliderを設置する
	/// (スケルトン有りの物に限る)
	/// </summary>
	class CharacterBoxCollider:public GameObject
	{
	public:
		using SettingTagFunc = std::function<int(const char* tag)>;
		/// <summary>
		/// 関係するボーンとボックスコライダーの構造体。
		/// </summary>
		struct RelationBB
		{
			Bone* bone;			//同期するボーン
			BoxCollider box;	//コライダー
			RigidBody rigid;	//剛体。
			CMatrix offsetmat = CMatrix::Identity();	//オフセット
		};
		//コンストラクタ
		CharacterBoxCollider() {};
		//デストラクタ
		~CharacterBoxCollider() {};
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
		/// <param name="path">ファイルパス</param>
		/// <param name="skeleton">スケルトン</param>
		/// <param name="scale">モデルの拡大率</param>
		/// <param name="func">剛体にタグを付けるための関数</param>
		void Init(const wchar_t* path, Skeleton* skeleton, float scale, SettingTagFunc func = [](auto tag) {return enCollisionAttr_NonHitIK;});
		/// <summary>
		/// スタート
		/// </summary>
		/// <returns></returns>
		bool Start() override;
		/// <summary>
		/// 更新。
		/// </summary>
		void Update() override;

	private:
		std::vector<RelationBB> m_relationBBList;		//関係するボーンとボックスコライダーの構造体のリスト
	};
}