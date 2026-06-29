#pragma once
#include "CharaManagerConst.h"

class Player;
class EnemyBase;
class BaseScene;

class CharaManager
{
public:

	//PreUpdate
	void CheckEnemyExpired();

	void SpawnPlayer(BaseScene* a_scene);
	void SpawnPlayer();

	void ResetPlayer(Math::Vector3 a_respawnPos);
	void ResetPlayer();
	std::shared_ptr<Player> GetPlayer()const { return m_player; }

	//召喚
	std::shared_ptr<EnemyBase> SpawnEnemy(EnemyName a_name, Math::Vector3 a_pos, float a_deg, float a_linePos);

	//敵リスト
	std::list<std::weak_ptr<EnemyBase>> GetEnemyList() { return m_wpEnemys; }

private:

	CharaManager() { Init(); }
	~CharaManager() { Release(); }

	void Init();
	void GeneratePlayer();
	void LoadPlayerTexData();
	void LoadEnemyTexData();
	void Release();

	std::shared_ptr<Player> m_player;
	std::list<std::weak_ptr<EnemyBase>> m_wpEnemys;
	std::map<PlayerAnimType, PolygonData> m_playerPolygonData;
	std::map<EnemyAnimType, PolygonData> m_enemyPolygonData[EnemyName::EnemyNumber];

	//敵の識別番号
	int m_enemyID = 0;

public:

	static CharaManager& Instance()
	{
		static CharaManager instance;
		return instance;
	}

};

#define CHARAMGR CharaManager::Instance()