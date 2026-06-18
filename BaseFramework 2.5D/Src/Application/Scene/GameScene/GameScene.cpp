#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Object/Character/Player/Player.h"
#include "../../Object/Character/CharaManager.h"
#include "../../StageSpawner/StageSpawner.h"
#include "../../Object/Sprite/HeightsNumber/HeightsNumber.h"
#include "../../Object/Sprite/KillCount/KillCount.h"
#include "../Transition/Transition.h"
#include "../../Sound/SoundManager.h"
#include "../../Score/ScoreManager.h"

void GameScene::Event()
{
	//キャラ更新
	CHARAMGR.Update();

	//ステージ更新
	STAGESPAWNER.Update();

	if (!m_wpPlayer.expired())
	{
		std::shared_ptr<Player> player = m_wpPlayer.lock();

		if (player->GetIsGameEnd() && !m_isTransition)
		{
			std::shared_ptr <Transition> transition = std::make_shared<Transition>();
			AddObject(transition);
			m_wpTransition = transition;
			m_isTransition = true;
		}

		if (m_isTransition)
		{
			if (!m_wpTransition.expired())
			{
				if (m_wpTransition.lock()->GetIsReset())
				{
					STAGESPAWNER.ResetStage();
					player->SetIsHeartTex(false);
					SCENEMGR.AddCarryObject(m_wpTransition.lock());
					SceneManager::Instance().SetNextScene
					(
						SceneManager::SceneType::Result
					);

					//サウンドの停止
					SOUNDMGR.Stop(SoundName::BGM_Ingame);
				}
			}
		}
	}

	if (GetAsyncKeyState('H') & 0x8000)
	{
		m_cameraPos.x += 0.2f;
	}
	if (GetAsyncKeyState('F') & 0x8000)
	{
		m_cameraPos.x -= 0.2f;
	}
	if (GetAsyncKeyState('Y') & 0x8000)
	{
		m_cameraPos.y += 0.2f;
	}
	if (GetAsyncKeyState('V') & 0x8000)
	{
		m_cameraPos.y -= 0.2f;
	}
	if (GetAsyncKeyState('T') & 0x8000)
	{
		m_cameraPos.z += 0.2f;
	}
	if (GetAsyncKeyState('B') & 0x8000)
	{
		m_cameraPos.z -= 0.2f;
	}
	KdDebugGUI::Instance().AddLog("CameraMoveCenter : G\n");
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_cameraDeg.x -= 0.5f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_cameraDeg.x += 0.5f;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_cameraDeg.y -= 0.5f;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_cameraDeg.y += 0.5f;
	}
	if (m_cameraDeg.x > 360.0f)m_cameraDeg.x -= 360.0f;
	else if (m_cameraDeg.x < 0.0f)m_cameraDeg.x += 360.0f;
	if (m_cameraDeg.y > 360.0f)m_cameraDeg.y -= 360.0f;
	else if (m_cameraDeg.y < 0.0f)m_cameraDeg.y += 360.0f;
	
	//仮セーブ
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		//一旦ストップ
		if (0)
		{
			FILE* fp = nullptr;

			if (fopen_s(&fp, "Asset/Data/Test/TestCameraData.csv", "w") == 0)
			{
				fprintf_s(fp, "%f,%f,%f,\n%f,%f,%f,",
					m_cameraPos.x,
					m_cameraPos.y,
					m_cameraPos.z,
					m_cameraDeg.x,
					m_cameraDeg.y,
					m_cameraDeg.z
				);

				fclose(fp);
			}
		}
	}

	Math::Matrix trans = Math::Matrix::CreateTranslation(m_cameraPos);
	Math::Matrix rotatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_cameraDeg.x));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_cameraDeg.y));
	Math::Matrix mat = rotatY * rotatX * trans;
	m_camera->SetCameraMatrix(mat);
}

void GameScene::Init()
{
	m_camera = std::make_shared<KdCamera>();
	m_camera->SetProjectionMatrix(90);
	
	//プレイヤー
	CHARAMGR.SpawnPlayer(this);
	CHARAMGR.GetPlayer()->SetIsHeartTex(true);
	m_wpPlayer = CHARAMGR.GetPlayer();

	//ステージ
	STAGESPAWNER.StartGame(this);

	//現在の高さ
	AddObject(std::make_shared<HeightsNumber>());

	//キルカウント
	AddObject(std::make_shared<KillCount>());

	//スコアリセット
	SCOREMGR.Reset();

	//テスト用
	FILE* fp = nullptr;
	
	if (fopen_s(&fp, "Asset/Data/Test/TestCameraData.csv", "r") == 0)
	{
		fscanf_s(fp, "%f,%f,%f,",
			&m_cameraPos.x,
			&m_cameraPos.y,
			&m_cameraPos.z
		);

		char dummy[250];
		fgets(dummy, 250, fp);

		fscanf_s(fp, "%f,%f,%f,",
			&m_cameraDeg.x,
			&m_cameraDeg.y,
			&m_cameraDeg.z
		);

		fclose(fp);
	}

	if (0)
	{
		m_cameraPos = Math::Vector3(0.0f, 5.0f, -45.0f);
		m_cameraDeg = Math::Vector3(0.0f, 0.0f, 0.0f);
	}

	//サウンド再生(仮)
	SOUNDMGR.Play(SoundName::BGM_Ingame);
}
