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
		static GameObjectManager* Get()
		{
			static GameObjectManager* ins = NULL;
			if (ins == NULL)
			{
				ins = new GameObjectManager();
			}
			return ins;
		}

		template<class T>
		T* TakeTrash()
		{
			T* t = NULL;
			for (GameObject* tr : mTrashBox)
			{
				t = dynamic_cast<T*>(tr);
				if (t != NULL)
				{
					break;
				}
			}
			if (t != NULL)
			{
				auto it = std::find(mTrashBox.begin(), mTrashBox.end(), t);
				mTrashBox.erase(it);
				//ob = (GameObject*)(t);
				t->Revive();
			}
			return t;
		}

		template<class T>
		T* NewGameObject(int prio, const char* name,bool isCheckIned)
		{
			if (prio >= mMaxSize)
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
			mGameObjectList[prio].push_back((GameObject*)go);
			if (isCheckIned && mCheckInCountList[prio]>0)
				mCheckInCountList[prio] -= 1;
			return go;
		}

		void DeleteGameObject(GameObject* go);

		void Update();
		void UpdateStart();
		void UpdateUpdate();
		void UpdateRender();
		void UpdatePostRender();

		void AddReserved(int prio, int add);
		int Capacity(int prio);
		void ClearCapacity(int prio);
		void CheckIN(int prio, int count);
	private:
		static const size_t mMaxSize = 32;
		std::array<std::vector<GameObject*>,mMaxSize> mGameObjectList;
		
		std::array<int, mMaxSize> mCheckInCountList;
		std::vector<GameObject*> mTrashBox;
	};

	template<class T>
	static T* NewGO(int prio, const char* name,bool isTakeTrush=true)
	{
		return GameObjectManager::Get()->NewGameObject<T>(prio, name, isTakeTrush);
	}
}