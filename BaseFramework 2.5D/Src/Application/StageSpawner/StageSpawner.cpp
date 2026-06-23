#include "StageSpawner.h"
#include "../Scene/SceneManager.h"
#include "../Scene/BaseScene/BaseScene.h"
#include "../Const/StageConst.h"
#include "StageObjectInclude.h"
#include "../Object/Character/CharaManager.h"

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
	noSpawnFlg = false;
	noSpawnStairCnt = 0;
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
		//未来のスポーン情報
		while (m_stairVisibleLog_future.size() < MAXFUTUREROLL)
		{
			//テスト
			if (rand() % 5 == 0 && !noSpawnFlg)
			{
				noSpawnFlg = true;
				noSpawnStairCnt = rand() % 2 + 2;
			}

			m_stairVisibleLog_future.push_back(noSpawnFlg && noSpawnStairCnt > 0 ? true : false);

			if (noSpawnFlg)
			{
				noSpawnStairCnt--;
				if (noSpawnStairCnt == -4)noSpawnFlg = false;
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
					if (rand() % 70 == 0)
					{
						float angleDeg = LOWEST->GetAngleDeg();
						float linePos = rand() / (float)RAND_MAX * (LINEPLAYAREA_MAX - LINEPLAYAREA_MIN - 3.2f) + LINEPLAYAREA_MIN + 1.6f;
						Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
						SCENEMGR.AddObject(std::make_shared<Boulder>(pos, angleDeg, linePos));
					}
					else if (rand() % 40 == 0)
					{
						float scale = (rand() % 4 + 1) * 0.5f;
						float angleDeg = LOWEST->GetAngleDeg();
						float linePos = rand() / (float)RAND_MAX * (LINEPLAYAREA_MAX - LINEPLAYAREA_MIN - 3.2f) + LINEPLAYAREA_MIN + 1.6f;
						Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
						SCENEMGR.AddObject(std::make_shared<RollingWood>(pos, angleDeg, linePos, scale));
					}

					//目
					if (rand() % 15 == 0)
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
						if (rand() % 12 == 0 && !isEnemySpawn)
						{
							float angleDeg = LOWEST->GetAngleDeg();
							float linePos = rand() / (float)RAND_MAX * (LINEPLAYAREA_MAX - LINEPLAYAREA_MIN - 3.2f) + LINEPLAYAREA_MIN + 1.6f;
							Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * linePos,respawnPosY,cosf(DirectX::XMConvertToRadians(angleDeg)) * linePos };
							CHARAMGR.SpawnEnemy(EnemyName_Goblin, pos, angleDeg, linePos);
						}
						else if (rand() % 10 == 0 && canSpear)
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
							if (rand() % 3 == 0)
							{
								m_isSideSpearNext = true;
							}
						}
						else if (rand() % 20 == 0 && canSpear)
						{
							m_isSideSpearNext = true;
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

void StageSpawner::Release()
{
}
