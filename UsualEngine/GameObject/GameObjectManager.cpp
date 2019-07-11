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
	}

	void GameObjectManager::UpdateStart()
	{
		for (auto goList : mGameObjectList)
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
		go->OnDestroy();
		go->DEAD();
	}

	void GameObjectManager::UpdateUpdate()
	{
		std::vector<DeadData> ddList;
		int Count = 0;
		for (auto goList : mGameObjectList)
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
					}
				}
				Count++;
			}
			Count = 0;
		}

		for (auto dd : ddList)
		{
			GameObject* go = mGameObjectList[dd.prio][dd.ind];
			auto it = std::find(mGameObjectList[dd.prio].begin(), mGameObjectList[dd.prio].end(), go);
			mGameObjectList[dd.prio].erase(it);
			if (go->IsTrashTake())
				mTrashBox.push_back(go);
			else
				delete go;
		}
	}

	void GameObjectManager::UpdateRender()
	{
	}

	void GameObjectManager::UpdatePostRender()
	{
	}

	void GameObjectManager::AddReserved(int prio, int add)
	{
		mGameObjectList[prio].reserve(mGameObjectList[prio].capacity() + add);
	}
	int GameObjectManager::Capacity(int prio)
	{
		int cap = mGameObjectList[prio].capacity();
		cap -= mCheckInCountList[prio];
		return cap;
	}
	void GameObjectManager::ClearCapacity(int prio)
	{
		mCheckInCountList[prio] = 0;
		mGameObjectList[prio].shrink_to_fit();
	}
	void GameObjectManager::CheckIN(int prio, int count)
	{
		mCheckInCountList[prio] += count;
	}
}