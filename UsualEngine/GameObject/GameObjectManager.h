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
			static GameObjectManager ins;
			
			return &ins;
		}

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
			m_gameObjectList[prio].push_back((GameObject*)go);
			if (isCheckIned && m_checkInCountList[prio]>0)
				m_checkInCountList[prio] -= 1;
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
		static const size_t m_maxSize = 32;
		std::array<std::vector<GameObject*>,m_maxSize> m_gameObjectList;
		
		std::array<int, m_maxSize> m_checkInCountList;
		std::vector<DeadData> m_ddList;
		std::vector<GameObject*> m_trashBox;
	};

	template<class T>
	static T* NewGO(int prio, const char* name="",bool isTakeTrush=true)
	{
		return GameObjectManager::Get()->NewGameObject<T>(prio, name, isTakeTrush);
	}
	
	template<class T>
	static void DeleteGO(T*& obj)
	{
		GameObjectManager::Get()->DeleteGameObject(obj);
		obj = nullptr;
	}
}