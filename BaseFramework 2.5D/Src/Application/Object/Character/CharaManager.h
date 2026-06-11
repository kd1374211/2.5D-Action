#pragma once
#include "CharaManagerConst.h"

class Player;
class BaseScene;

class CharaManager
{
public:

	void StartGame(BaseScene* a_scene);

	void Update();

	std::shared_ptr<Player> GetPlayer()const { return m_player; }

private:

	CharaManager() { Init(); }
	~CharaManager() { Release(); }

	void Init();
	void MakeShared();
	void LoadPlayerTexData();
	void Release();

	std::shared_ptr<Player> m_player;
	std::map<PlayerAnimType, PolygonData> m_playerPolygonData;

public:

	static CharaManager& Instance()
	{
		static CharaManager instance;
		return instance;
	}

};

#define CHARAMGR CharaManager::Instance()