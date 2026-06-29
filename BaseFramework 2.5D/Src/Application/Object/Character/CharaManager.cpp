#include "CharaManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/BaseScene/BaseScene.h"
#include "CharacterInclude.h"

void CharaManager::CheckEnemyExpired()
{
	auto it = m_wpEnemys.begin();

	while (it != m_wpEnemys.end())
	{
		if (it->expired())
		{
			it = m_wpEnemys.erase(it);
		}
		else
		{
			it++;
		}
	}

	//デバッグ
	KdDebugGUI::Instance().AddLog("Enemys : %d\n", m_wpEnemys.size());
}

void CharaManager::SpawnPlayer(BaseScene* a_scene)
{
	a_scene->AddObject(m_player);
}

void CharaManager::SpawnPlayer()
{
	SCENEMGR.AddObject(m_player);
}

void CharaManager::ResetPlayer(Math::Vector3 a_respawnPos)
{
	m_player->Respawn(a_respawnPos);
}

void CharaManager::ResetPlayer()
{
	m_player->Respawn();
}

std::shared_ptr<EnemyBase> CharaManager::SpawnEnemy(EnemyName a_name, Math::Vector3 a_pos, float a_deg, float a_linePos)
{
	std::shared_ptr<EnemyBase>enemy = nullptr;
	switch (a_name)
	{
	case EnemyName::EnemyName_Goblin:
		enemy = std::make_shared<Goblin>(a_pos, a_deg, a_linePos);
		enemy->SetEnemyID(m_enemyID);
		break;
	case EnemyName::EnemyName_FlyEye:
		enemy = std::make_shared<FlyEye>(a_pos, a_deg, a_linePos);
		enemy->SetEnemyID(m_enemyID);
		break;
	}

	enemy->SetPolygonData(&m_enemyPolygonData[(int)a_name]);
	SCENEMGR.AddObject(enemy);
	m_wpEnemys.push_back(enemy);

	m_enemyID++;
	if (m_enemyID >= 10000)m_enemyID -= 10000;

	return enemy;
}

void CharaManager::Init()
{
	LoadPlayerTexData();
	GeneratePlayer();

	LoadEnemyTexData();
}

void CharaManager::LoadPlayerTexData()
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, "Asset/Data/Chara/PlayerTexData.csv", "r") == 0)
	{
		const int STRLENG = 100;
		Math::Vector2 rect = Math::Vector2::Zero;
		float scaleDiv = 1.0f;
		char dummy[STRLENG] = {};

		fgets(dummy, STRLENG, fp);
		fscanf_s(fp, "%f,%f,%f", &rect.x, &rect.y, &scaleDiv);
		fgets(dummy, STRLENG, fp);

		for (int i = 0; i < (int)PlayerAnimType::PlayerAnimNumber; i++)
		{
			if (fgets(dummy, STRLENG, fp) != nullptr)//1行読み
			{
				char path[STRLENG] = {};
				float animMax = 0.0f;
				float animAdd = 0.0f;

				fscanf_s(fp, "%[^,],%[^,],%f,%f,",
					dummy, STRLENG,
					path, STRLENG,
					&animMax,
					&animAdd);

				std::shared_ptr<KdSquarePolygon> polygon = std::make_shared<KdSquarePolygon>();
				polygon->SetMaterial(path);
				polygon->SetSplit((int)animMax, 1);
				polygon->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
				polygon->SetScale(rect / scaleDiv);
				PolygonData data = { animMax,animAdd,polygon };

				m_playerPolygonData.emplace((PlayerAnimType)i, data);
			}
		}

		fclose(fp);
	}
}

void CharaManager::LoadEnemyTexData()
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, "Asset/Data/Chara/EnemyTexData.csv", "r") == 0)
	{
		const int STRLENG = 300;
		char dummy[STRLENG] = {};

		Math::Vector2 rect = Math::Vector2::Zero;
		float scaleDiv = 0.0f;
		KdSquarePolygon::PivotType pivot = KdSquarePolygon::PivotType::Center_Middle;
		char path[STRLENG] = {};
		float animMax = 0.0f;
		float animAdd = 0.0f;

		for (int i = 0; i < EnemyName::EnemyNumber; i++)
		{
			if (fgets(dummy, STRLENG, fp) != nullptr)//1行読み
			{
				fscanf_s(fp, "%[^,],%d,%f,%f,%f,",
					dummy, STRLENG, 
					&pivot,
					&rect.x,
					&rect.y,
					&scaleDiv);

				for (int j = 0; j < (int)EnemyAnimType::EnemyAnimNumber; j++)
				{
					fscanf_s(fp, "%[^,],%f,%f,",
						path, STRLENG,
						&animMax,
						&animAdd);

					std::shared_ptr<KdSquarePolygon> polygon = std::make_shared<KdSquarePolygon>();
					polygon->SetMaterial(path);
					polygon->SetSplit((int)animMax, 1);
					polygon->SetPivot(pivot);
					polygon->SetScale(rect / scaleDiv);
					PolygonData data = { animMax,animAdd,polygon };

					m_enemyPolygonData[i].emplace((EnemyAnimType)j, data);
				}
			}
		}

		fclose(fp);
	}
}

void CharaManager::GeneratePlayer()
{
	m_player = std::make_shared<Player>();
	m_player->SetPolygonData(&m_playerPolygonData);
}

void CharaManager::Release()
{}
