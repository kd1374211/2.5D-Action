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

	//召喚
	void SpawnEnemy(EnemyName a_name, Math::Vector3 a_pos, float a_deg, float a_linePos);

private:

	CharaManager() { Init(); }
	~CharaManager() { Release(); }

	void Init();
	void GeneratePlayer();
	void LoadPlayerTexData();
	void LoadEnemyTexData();
	void Release();

	std::shared_ptr<Player> m_player;
	std::map<PlayerAnimType, PolygonData> m_playerPolygonData;
	std::map<EnemyAnimType, PolygonData> m_enemyPolygonData[EnemyName::EnemyNumber];

public:

	static CharaManager& Instance()
	{
		static CharaManager instance;
		return instance;
	}

};

#define CHARAMGR CharaManager::Instance()