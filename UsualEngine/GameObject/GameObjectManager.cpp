#include "PreCompile.h"
#include "GameObjectManager.h"

#include "util/Util.h"

namespace UsualEngine
{
	GameObjectManager::GameObjectManager()
	{
	}

	GameObjectManager::~GameObjectManager()
	{
	}

	void GameObjectManager::Update()
	{
		UpdateStart();
		UpdateUpdate();
		UpdateRender();
		UpdatePostRender();

		Physics().DebugDraw();
	}

	void GameObjectManager::UpdateStart()
	{
		for (auto goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				if (!go->isStart() && !go->isDead())
				{
					if (go->Start())
						go->START();
				}
			}
		}
	}

	void GameObjectManager::DeleteGameObject(GameObject* go)
	{
		if (go == nullptr)
			return;
		go->OnDestroy();
		go->DEAD();
	}

	void GameObjectManager::UpdateUpdate()
	{
		int Count = 0;
		for (auto goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				if (go->isStart())
				{
					if(!go->isDead())
						go->Update();
					else
					{
						DeadData dd;
						dd.ind = Count;
						dd.prio = go->GetPrio();
						m_ddList.push_back(dd);
					}
				}
				Count++;
			}
			Count = 0;
		}

		for (auto dd : m_ddList)
		{
			GameObject* go = m_gameObjectList[dd.prio][dd.ind];
			auto it = std::find(m_gameObjectList[dd.prio].begin(), m_gameObjectList[dd.prio].end(), go);
			m_gameObjectList[dd.prio].erase(it);
			if (go->IsTrashTake())
				m_trashBox.push_back(go);
			else
				delete go;
		}
		m_ddList.clear();
	}

	void GameObjectManager::UpdateRender()
	{
		for (auto goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				go->Render();
			}
		}

		
	}

	void GameObjectManager::UpdatePostRender()
	{
		for (auto goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				go->PostRender();
			}
		}
	}

	void GameObjectManager::AddReserved(int prio, int add)
	{
		m_gameObjectList[prio].reserve(m_gameObjectList[prio].capacity() + add);
	}
	int GameObjectManager::Capacity(int prio)
	{
		int cap = m_gameObjectList[prio].capacity();
		cap -= m_checkInCountList[prio];
		return cap;
	}
	void GameObjectManager::ClearCapacity(int prio)
	{
		m_checkInCountList[prio] = 0;
		m_gameObjectList[prio].shrink_to_fit();
	}
	void GameObjectManager::CheckIN(int prio, int count)
	{
		m_checkInCountList[prio] += count;
	}
}