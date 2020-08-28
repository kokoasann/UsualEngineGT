#pragma once

#include "GameObject.h"
namespace UsualEngine
{
	struct DeadData
	{
		int prio = 0;
		int ind = 0;
	};
	class GameObjectManager
	{
	private:
		GameObjectManager();
		~GameObjectManager();
	public:

		void Release();

		/// <summary>
		/// NewGOされたもののデータ。
		/// </summary>
		struct NewGOData
		{
			GameObject* go = nullptr;	//Newされたやつのポインタ
			char prio = 0;				//プライオリティ
		};
		static GameObjectManager* Get()
		{
			static GameObjectManager ins;
			
			return &ins;
		}

		/// <summary>
		/// ゴミ箱に入ってるオブジェクトを取り出す。
		/// </summary>
		/// <returns></returns>
		template<class T>
		T* TakeTrash()
		{
			T* t = NULL;
			for (GameObject* tr : m_trashBox)
			{
				t = dynamic_cast<T*>(tr);
				if (t != NULL)
				{
					break;
				}
			}
			if (t != NULL)
			{
				auto it = std::find(m_trashBox.begin(), m_trashBox.end(), t);
				m_trashBox.erase(it);
				//ob = (GameObject*)(t);
				t->Revive();
			}
			return t;
		}

		/// <summary>
		/// オブジェクトを生成する。
		/// </summary>
		/// <param name="prio">プライオリティ</param>
		/// <param name="name">名前</param>
		/// <param name="isCheckIned">チェックインしてる？</param>
		/// <returns></returns>
		template<class T>
		T* NewGameObject(int prio, const char* name,bool isCheckIned)
		{
			if (prio >= m_maxSize)
			{
				//MessageBoxA(usualEngine()->GetHwnd(),"","",0)
				assert(0);
			}
			int hash = Util::MakeHash(name);

			T* go = TakeTrash<T>();
			if (go == nullptr)
			{
				go = new T();
			}

			GameObject* o = (GameObject*)go;
			o->Awake();
			o->SetPrio(prio);
			o->SetName(hash);
			o->CreatInGameObjectManager();
			//m_gameObjectList[prio].push_back((GameObject*)go);
			m_newGOBuffer.push_back({ go,(char)prio });
			if (isCheckIned && m_checkInCountList[prio]>0)
				m_checkInCountList[prio] -= 1;
			return go;
		}

		/// <summary>
		/// オブジェクトを消す
		/// オブジェクトが使いまわしできるようになっているものはデリートはされない。
		/// </summary>
		/// <param name="go">消すオブジェクト</param>
		void DeleteGameObject(GameObject* go);

		/// <summary>
		/// 更新。
		/// </summary>
		void Update();
		/// <summary>
		/// Startを更新
		/// </summary>
		void UpdateStart();
		/// <summary>
		/// Updateを更新
		/// </summary>
		void UpdateUpdate();
		/// <summary>
		/// PreRenderを更新
		/// </summary>
		void UpdatePreRender();
		/// <summary>
		/// Renderを更新
		/// </summary>
		void UpdateRender();
		/// <summary>
		/// PostRenderを更新
		/// </summary>
		void UpdatePostRender();

		/// <summary>
		/// オブジェクトリストのキャパシティをadd分増やす
		/// </summary>
		/// <param name="prio">プライオリティ</param>
		/// <param name="add">増やす数</param>
		void AddReserved(int prio, int add);
		/// <summary>
		/// オブジェクトリストのキャパシティを返す
		/// </summary>
		/// <param name="prio">プライオリティ</param>
		/// <returns></returns>
		int Capacity(int prio);
		/// <summary>
		/// オブジェクトリストの余分を消す。
		/// CheckINのカウントを0にする。
		/// </summary>
		/// <param name="prio">プライオリティ</param>
		void CapacityShrink2Fit(int prio);
		/// <summary>
		/// チェックイン
		/// </summary>
		/// <param name="prio">プライオリティ</param>
		/// <param name="count">カウント</param>
		void CheckIN(int prio, int count);
	private:
		static const size_t m_maxSize = 32;					//priorityの最大値。
		std::array<std::vector<GameObject*>,m_maxSize> m_gameObjectList;	//生きているゲームオブジェクトのリスト
		std::vector<NewGOData> m_newGOBuffer;				//そのフレームに生成されたオブジェクトのリスト
		
		std::array<int, m_maxSize> m_checkInCountList;		//
		std::vector<DeadData> m_ddList;						//DeleteGOされたオブジェクトのリスト
		std::vector<GameObject*> m_trashBox;				//再利用されるオブジェクトのリスト

		bool m_isReleaseProcess = false;					//全オブジェクトの開放処理中？
	};

	/// <summary>
	/// オブジェクトを生成する
	/// </summary>
	/// <param name="prio">プライオリティ</param>
	/// <param name="name">名前</param>
	/// <param name="isTakeTrush">使いまわしする</param>
	/// <returns></returns>
	template<class T>
	static T* NewGO(int prio, const char* name="",bool isTakeTrush=true)
	{
		return GameObjectManager::Get()->NewGameObject<T>(prio, name, isTakeTrush);
	}
	
	/// <summary>
	/// オブジェクトを消す
	/// 使いまわしをする奴はデリートをしない
	/// </summary>
	/// <param name="obj">消すオブジェクト</param>
	template<class T>
	static void DeleteGO(T*& obj)
	{
		GameObjectManager::Get()->DeleteGameObject(obj);
		obj = nullptr;
	}
}