#include "StageSpawner.h"
#include "../Scene/SceneManager.h"
#include "../Scene/BaseScene/BaseScene.h"
#include "../Const/StageConst.h"
#include "StageObjectInclude.h"
#include "../Object/Character/CharaManager.h"
#include "../Score/ScoreManager.h"
#include "../Object/Character/Enemy/EnemyBase.h"

//階段再出現用
#define LOWEST m_stairs.front()
#define HIGHEST m_stairs.back()

void StageSpawner::StartGame(BaseScene* a_scene)
{
	//リストの内容を転送
	for (auto& itr : m_stairs)
	{
		a_scene->AddObject(itr);
	}

	//柱と壁
	a_scene->AddObject(m_pillar);
	a_scene->AddObject(m_wall);

	//それとは別に奈落召喚
	a_scene->AddObject(std::make_shared<Void>());
}

void StageSpawner::ResetStage()
{
	//フラグ初期化s
	for (auto &itr : m_stairs)
	{
		itr->SetStairFlg(true);
	}
	m_stairVisibleLog_future.clear();
	m_stairVisibleLog_past.clear();
	m_countBackScroll = 0;
	m_isSideSpearNext = false;
	m_stairSpawnFlg = true;
	m_noSpawnStairCnt = 0;
	m_level = 0;
	m_heartSpawnCnt = 0;
	m_heartSpawnChance = HEARTSPAWNCHANCESTART;
	m_woodRushCnt = 0;
	m_woodCool = 0;
}

void StageSpawner::Update()
{
	switch (SCENEMGR.GetSceneType())
	{
	case SceneManager::SceneType::Title:
	case SceneManager::SceneType::Result:
		//準方向スクロール
		if (SCENEMGR.GetFrameScroll() >= 0.0f)
		{
			//Front（一番下）のワープフラグがある間
			while (LOWEST->GetIsDisappear())
			{
				//ワープ
				LOWEST->Respawn(true);

				//今ワープした物を階段リストのBackに移動
				m_stairs.push_back(LOWEST);
				m_stairs.pop_front();
			}
		}
		else
		{
			//Back（一番上）のワープフラグがある間
			while (HIGHEST->GetIsDisappear())
			{
				//ワープ
				HIGHEST->Respawn(true);

				//今ワープした物を階段リストのFrontに移動
				m_stairs.push_front(HIGHEST);
				m_stairs.pop_back();
			}
		}
		if (m_pillar->GetIsDisappear())m_pillar->Respawn();
		if (m_wall->GetIsDisappear())m_wall->Respawn();
		break;
	case SceneManager::SceneType::Game:
		//レベル更新
		if (m_level < MAXLEVEL - 1)
		{
			if (SCOREMGR.GetCurrentHeight() > LEVELHEIGHTS[m_level + 1])
			{
				m_level++;
			}
		}

		//未来のスポーン情報
		while (m_stairVisibleLog_future.size() < MAXFUTUREROLL)
		{
			//数更新
			if (m_noSpawnStairCool > 0)m_noSpawnStairCool--;
			else
			{
				//出現ガチャ
				if (rand() % m_noStairsData[m_level].m_chance == 0 && m_stairSpawnFlg)
				{
					m_stairSpawnFlg = false;
					m_noSpawnStairCnt = rand() % 100 < m_noStairsData[m_level].m_tripleChance ? 3 : 2;
				}
			}

			m_stairVisibleLog_future.push_back(m_stairSpawnFlg);

			if (!m_stairSpawnFlg)
			{
				m_noSpawnStairCnt--;
				if (m_noSpawnStairCnt <= 0)
				{
					m_stairSpawnFlg = true;
					m_noSpawnStairCool = m_noStairsData[m_level].m_cool;
				}
			}
		}

		//仮置き（階段があるかどうか）
		bool isStair = true;

		//ワープ後に取得する座標
		float respawnPosY = 0.0f;

		//敵がスポーンしたか
		bool isEnemySpawn = false;

		//準方向スクロール
		if (SCENEMGR.GetFrameScroll() >= 0.0f)
		{
			//Front（一番下）のワープフラグがある間
			while (LOWEST->GetIsDisappear())
			{
				//ワープ
				LOWEST->Respawn();

				//ワープ後の座標取得
				respawnPosY = LOWEST->GetPos().y;

				//次に出現する階段のデータを取得（あり得ないが空ならfalse）
				if (!m_stairVisibleLog_future.empty())
				{
					isStair = m_stairVisibleLog_future.front();
					m_stairVisibleLog_future.pop_front();
				}

				//戻り数カウントが0でないなら減少
				if (m_countBackScroll > 0)m_countBackScroll--;
				//0ならギミックの出現チェック
				else
				{
					GimmicksData data = {};

					//ギミッククール減少
					if (m_boulderCool > 0)m_boulderCool--;

					//岩
					data = m_gimmicksData.find(Gimmicks::Boulder)->second;
					if (rand() % data.m_chance[m_level] == 0 && m_level >= data.m_minLevel && m_boulderCool <= 0)
					{
						float angleDeg = LOWEST->GetAngleDeg();
						float linePos = rand() / (float)RAND_MAX * (data.m_linePosMax - data.m_linePosMin) + data.m_linePosMin;
						Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
						SCENEMGR.AddObject(std::make_shared<Boulder>(pos, angleDeg, linePos));
						m_boulderCool = BOULDERCOOL;
					}
					else
					{
						//このフレームで木が出現したか
						bool isWoodSpawned = false;

						//連続出現防止処理
						if (m_woodCool <= 0)
						{
							for (int i = (int)Gimmicks::Wood_05; i < (int)Gimmicks::Wood_20; i++)
							{
								//木
								data = m_gimmicksData.find((Gimmicks)i)->second;
								if (rand() % data.m_chance[m_level] == 0 && m_level >= data.m_minLevel)
								{
									float scale = (i - (int)Gimmicks::Wood_05 + 1) * 0.5f;
									float angleDeg = LOWEST->GetAngleDeg();						
									float linePos = rand() / (float)RAND_MAX * (data.m_linePosMax - data.m_linePosMin) + data.m_linePosMin;
									Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
									SCENEMGR.AddObject(std::make_shared<RollingWood>(pos, angleDeg, linePos, scale));
									isWoodSpawned = true;
									break;
								}
							}
						}
						else m_woodCool--;

						if (isWoodSpawned)
						{
							m_woodRushCnt++;
							if (m_woodRushCnt >= WOODRUSHLIMIT)m_woodCool = WOODCOOL;
						}
						else m_woodRushCnt = 0;
					}
						

					//目
					data = m_gimmicksData.find(Gimmicks::FlyEye)->second;
					if (rand() % data.m_chance[m_level] == 0 && m_level >= data.m_minLevel)
					{
						float angleDeg = LOWEST->GetAngleDeg();
						float linePos = rand() / (float)RAND_MAX * (data.m_linePosMax - data.m_linePosMin) + data.m_linePosMin;
						Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
						std::shared_ptr<EnemyBase>enemy = CHARAMGR.SpawnEnemy(EnemyName_FlyEye, pos, angleDeg, linePos);

						//ID結びつけ
						m_enemyStairMap.emplace(enemy->GetEnemyID(), LOWEST->GetStairID());

						isEnemySpawn = true;
					}

					static int spearWait = 0;
					bool canSpear = spearWait <= 0 ? true : false;
					if (spearWait > 0)spearWait--;

					if (m_isSideSpearNext)
					{
						if (m_stairVisibleLog_future.front())
						{
							float angleDeg = LOWEST->GetAngleDeg();
							SCENEMGR.AddObject(std::make_shared<SideSpear>(respawnPosY, angleDeg));
						}
						m_isSideSpearNext = false;
						spearWait++;
					}
					//階段があるかを見る
					else if (isStair)
					{
						//ゴブリン
						data = m_gimmicksData.find(Gimmicks::Goblin)->second;
						if (rand() % data.m_chance[m_level] == 0 && !isEnemySpawn && m_level >= data.m_minLevel)
						{
							float angleDeg = LOWEST->GetAngleDeg();
							float linePos = rand() / (float)RAND_MAX * (data.m_linePosMax - data.m_linePosMin) + data.m_linePosMin;
							Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
							std::shared_ptr<EnemyBase>enemy = CHARAMGR.SpawnEnemy(EnemyName_Goblin, pos, angleDeg, linePos);

							//ID結びつけ
							m_enemyStairMap.emplace(enemy->GetEnemyID(), LOWEST->GetStairID());
						}
						else
						{
							//槍
							data = m_gimmicksData.find(Gimmicks::Spear)->second;
							if (rand() % data.m_chance[m_level] == 0 && canSpear && m_level >= data.m_minLevel)
							{
								float angleDeg = LOWEST->GetAngleDeg();
								float linePos = rand() / (float)RAND_MAX * (data.m_linePosMax - data.m_linePosMin) + data.m_linePosMin;

								for (int i = 0; i < 5; i++)
								{
									Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
									SCENEMGR.AddObject(std::make_shared<Spear>(pos, angleDeg, linePos));
									linePos += 0.5f;
									spearWait++;
								}

								//横槍
								data = m_gimmicksData.find(Gimmicks::SpearAfterSide)->second;
								if (rand() % data.m_chance[m_level] == 0 && m_level >= data.m_minLevel)
								{
									m_isSideSpearNext = true;
								}
							}
							else
							{
								//横槍
								data = m_gimmicksData.find(Gimmicks::SideSpear)->second;
								if (rand() % data.m_chance[m_level] == 0 && canSpear && m_level >= data.m_minLevel)
								{
									m_isSideSpearNext = true;
								}
							}
						}
					}
				}

				//透明フラグ設定
				LOWEST->SetStairFlg(isStair);

				//今ワープした物を階段リストのBackに移動
				m_stairs.push_back(LOWEST);
				m_stairs.pop_front();
			}
		}
		//逆方向スクロール
		else
		{
			//Back（一番上）のワープフラグがある間
			while (HIGHEST->GetIsDisappear())
			{
				//戻り数カウント増加
				m_countBackScroll++;

				//過去に消えた階段のログが残っているならそれを使う
				if (!m_stairVisibleLog_past.empty())
				{
					isStair = m_stairVisibleLog_past.front();
					m_stairVisibleLog_past.pop_front();
				}

				//ワープ
				HIGHEST->Respawn(isStair);

				//今ワープした物を階段リストのFrontに移動
				m_stairs.push_front(HIGHEST);
				m_stairs.pop_back();
			}
		}

		if (m_pillar->GetIsDisappear())m_pillar->Respawn();
		if (m_wall->GetIsDisappear())m_wall->Respawn();

		break;
	}

	KdDebugGUI::Instance().AddLog("EnemyMap : %d\n", m_enemyStairMap.size());
	KdDebugGUI::Instance().AddLog("HeartChance : %d\n", m_heartSpawnChance);
}

void StageSpawner::AddPastStairVisibleLog(bool a_isVisible)
{
	m_stairVisibleLog_past.push_front(a_isVisible);
	if (m_stairVisibleLog_past.size() > MAXDATASTORE)m_stairVisibleLog_past.pop_back();
}

void StageSpawner::AddFutureStairVisibleLog(bool a_isVisible)
{
	m_stairVisibleLog_future.push_front(a_isVisible);
}

void StageSpawner::OnEnemyDead(int a_enemyID)
{
	//最初にロール
	if (rand() % m_heartSpawnChance == 0)
	{
		//対応検索
		auto itr = m_enemyStairMap.find(a_enemyID);
		if (itr != m_enemyStairMap.end())
		{
			//初期化
			std::shared_ptr<EnemyBase> enemy = nullptr;
			std::shared_ptr<Stair> stair = nullptr;

			//敵サーチ
			for (auto& objEnemy : CHARAMGR.GetEnemyList())
			{
				if (objEnemy.expired())continue;

				if (objEnemy.lock()->GetEnemyID() == a_enemyID)
				{
					//敵取得
					enemy = objEnemy.lock();
					break;
				}
			}

			//見つかったら
			if (enemy != nullptr)
			{
				//N個先の階段を取得
				int number = HEARTFLYMIN;

				while (1)
				{
					int target = itr->second + number;

					//ループ
					if (target >= STAIRNUM)target -= STAIRNUM;

					//階段があるかを取得
					std::weak_ptr<Stair> targetStair = m_stairsIDmap.find(target)->second;
					if (targetStair.expired())continue;

					//階段取得
					stair = targetStair.lock();

					if (stair->GetStairFlg())
					{
						//LinePosランダム
						float linePos = rand() / (float)RAND_MAX * (LINEPLAYAREA_MAX - LINEPLAYAREA_MIN - 1.2f) + LINEPLAYAREA_MIN + 0.6f;

						//敵の位置からハートを召喚
						SCENEMGR.AddObject(std::make_shared<Heart>(enemy->GetPos(), enemy->GetAngleDeg(), enemy->GetLinePos(), linePos, number));

						//ハートの召喚数追加
						m_heartSpawnCnt++;
						//それ倍の確率を追加
						m_heartSpawnChance += HEARTSPAWNCHANCEADD * m_heartSpawnCnt;

						return;
					}

					number++;
					if (number >= HEARTFLYMAX)return;
				}
			}
		}
	}
	else
	{
		//次が出やすく
		m_heartSpawnChance--;
	}
}

void StageSpawner::DeleteEnemyMap(int a_enemyID)
{
	auto itr = m_enemyStairMap.find(a_enemyID);
	if (itr != m_enemyStairMap.end())
	{
		m_enemyStairMap.erase(itr);
	}
}

void StageSpawner::Init()
{
	//データ
	LoadData();

	//階段召喚
	for (int i = 0; i < STAIRNUM; i++)
	{
		float angleDeg = STAIRDEGBASE - (float)i * STAIRDEGDIFF;
		Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * STAIRSPAWNRANGE,STAIRSPAWNBASE_Y + (float)i * STAIRSPAWNDIFF_Y,cosf(DirectX::XMConvertToRadians(angleDeg)) * STAIRSPAWNRANGE };
		std::shared_ptr<Stair> stair = std::make_shared<Stair>(pos, angleDeg, i);
		m_stairs.push_back(stair);

		//対応マップ生成
		m_stairsIDmap.emplace(stair->GetStairID(), stair);
	}

	//壁柱召喚
	m_wall = std::make_shared<Wall>(Math::Vector3::Zero);
	m_pillar = std::make_shared<Pillar>(Math::Vector3::Zero);
}

void StageSpawner::LoadData()
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, "Asset/Data/Stage/GimmicksData.csv", "r") == 0)
	{
		const int STRLENG = 100;
		char dummy[STRLENG] = {};
		int level = 0;

		//階段未出現ロールチャンス
		for (int i = 0; i < MAXLEVEL; i++)
		{
			if (fgets(dummy, STRLENG, fp) != nullptr)
			{
				fscanf_s(fp, "%d,%d,%d,%d,",
					&level,
					&m_noStairsData[i].m_chance,
					&m_noStairsData[i].m_cool,
					&m_noStairsData[i].m_tripleChance);
			}
		}
		
		//読んどく
		fgets(dummy, STRLENG, fp);

		int index = 0;
		GimmicksData data = {};

		for (int i = 0; i < (int)Gimmicks::Max; i++)
		{
			if (fgets(dummy, STRLENG, fp) != nullptr)//1行読み
			{
				fscanf_s(fp, "%d,%f,%f,%d,",
					&index,
					&data.m_linePosMin,
					&data.m_linePosMax,
					&data.m_minLevel);

				for (int j = 0; j < MAXLEVEL; j++)
				{
					fscanf_s(fp, "%d,",
						&data.m_chance[j]);
				}

				m_gimmicksData.emplace((Gimmicks)index, data);
			}
		}

		fclose(fp);
	}
}

void StageSpawner::Release()
{
}
