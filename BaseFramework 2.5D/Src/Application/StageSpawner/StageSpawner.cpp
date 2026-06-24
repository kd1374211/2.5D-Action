#include "StageSpawner.h"
#include "../Scene/SceneManager.h"
#include "../Scene/BaseScene/BaseScene.h"
#include "../Const/StageConst.h"
#include "StageObjectInclude.h"
#include "../Object/Character/CharaManager.h"
#include "../Score/ScoreManager.h"

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
		itr->SetStairFlg(false);
	}
	m_stairVisibleLog_future.clear();
	m_stairVisibleLog_past.clear();
	m_countBackScroll = 0;
	m_isSideSpearNext = false;
	m_noSpawnFlg = false;
	m_noSpawnStairCnt = 0;
	m_level = 0;
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
				LOWEST->Respawn(false);

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
				HIGHEST->Respawn(false);

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
		if (m_level > 0)
		{
			//レベルダウン
			if (SCOREMGR.GetCurrentHeight() < LEVELHEIGHTS[m_level])
			{
				m_level--;
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
				if (rand() % m_noStairsData[m_level].m_chance == 0 && !m_noSpawnFlg)
				{
					m_noSpawnFlg = true;
					m_noSpawnStairCnt = rand() % 100 < m_noStairsData[m_level].m_tripleChance ? 3 : 2;
				}
			}

			m_stairVisibleLog_future.push_back(m_noSpawnFlg);

			if (m_noSpawnFlg)
			{
				m_noSpawnStairCnt--;
				if (m_noSpawnStairCnt <= 0)
				{
					m_noSpawnFlg = false;
					m_noSpawnStairCool = m_noStairsData[m_level].m_cool;
				}
			}
		}

		//仮置き（階段があるかどうか）
		bool isStair = false;

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
						float linePos = rand() / (float)RAND_MAX * (LINEPLAYAREA_MAX - LINEPLAYAREA_MIN - 2.6f) + LINEPLAYAREA_MIN + 1.3f;
						Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
						SCENEMGR.AddObject(std::make_shared<Boulder>(pos, angleDeg, linePos));
						m_boulderCool = BOULDERCOOL;
					}
					else
					{
						for (int i = (int)Gimmicks::Wood_05; i < (int)Gimmicks::Wood_20; i++)
						{
							//木
							data = m_gimmicksData.find((Gimmicks)i)->second;
							if (rand() % data.m_chance[m_level] == 0 && m_level >= data.m_minLevel)
							{
								float scale =( i - (int)Gimmicks::Wood_05 + 1) * 0.5f;
								float angleDeg = LOWEST->GetAngleDeg();
								float linePos = rand() / (float)RAND_MAX * (LINEPLAYAREA_MAX - LINEPLAYAREA_MIN - 3.2f) + LINEPLAYAREA_MIN + 1.6f;
								Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
								SCENEMGR.AddObject(std::make_shared<RollingWood>(pos, angleDeg, linePos, scale));
								break;
							}
						}
					}
						

					//目
					data = m_gimmicksData.find(Gimmicks::FlyEye)->second;
					if (rand() % data.m_chance[m_level] == 0 && m_level >= data.m_minLevel)
					{
						float angleDeg = LOWEST->GetAngleDeg();
						float linePos = rand() / (float)RAND_MAX * (LINEPLAYAREA_MAX - LINEPLAYAREA_MIN - 3.2f) + LINEPLAYAREA_MIN + 1.6f;
						Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
						CHARAMGR.SpawnEnemy(EnemyName_FlyEye, pos, angleDeg, linePos);

						isEnemySpawn = true;
					}

					static int spearWait = 0;
					bool canSpear = spearWait <= 0 ? true : false;
					if (spearWait > 0)spearWait--;

					if (m_isSideSpearNext)
					{
						if (!m_stairVisibleLog_future.front())
						{
							float angleDeg = LOWEST->GetAngleDeg();
							SCENEMGR.AddObject(std::make_shared<SideSpear>(respawnPosY, angleDeg));
						}
						m_isSideSpearNext = false;
						spearWait++;
					}
					//階段があるかを見る
					else if (!isStair)
					{
						//ゴブリン
						data = m_gimmicksData.find(Gimmicks::Goblin)->second;
						if (rand() % data.m_chance[m_level] == 0 && !isEnemySpawn && m_level >= data.m_minLevel)
						{
							float angleDeg = LOWEST->GetAngleDeg();
							float linePos = rand() / (float)RAND_MAX * (LINEPLAYAREA_MAX - LINEPLAYAREA_MIN - 3.2f) + LINEPLAYAREA_MIN + 1.6f;
							Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
							CHARAMGR.SpawnEnemy(EnemyName_Goblin, pos, angleDeg, linePos);
						}
						else
						{
							//槍
							data = m_gimmicksData.find(Gimmicks::Spear)->second;
							if (rand() % data.m_chance[m_level] == 0 && canSpear && m_level >= data.m_minLevel)
							{
								float angleDeg = LOWEST->GetAngleDeg();
								float linePos = rand() / (float)RAND_MAX * (LINEPLAYAREA_MAX - LINEPLAYAREA_MIN - 2.6f) + LINEPLAYAREA_MIN + 0.6f;

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

void StageSpawner::Init()
{
	//データ
	LoadData();

	//階段召喚
	for (int i = 0; i < STAIRNUM; i++)
	{
		float angleDeg = STAIRDEGBASE - (float)i * STAIRDEGDIFF;
		Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * STAIRSPAWNRANGE,STAIRSPAWNBASE_Y + (float)i * STAIRSPAWNDIFF_Y,cosf(DirectX::XMConvertToRadians(angleDeg)) * STAIRSPAWNRANGE };
		m_stairs.push_back(std::make_shared<Stair>(pos, angleDeg));
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
