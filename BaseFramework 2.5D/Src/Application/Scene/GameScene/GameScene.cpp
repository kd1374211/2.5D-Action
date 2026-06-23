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
#include "../../Camera/CameraManager.h"

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

	//データ回収
	CameraBaseData data = CAMERAMGR.GetCameraData();

	Math::Matrix trans = Math::Matrix::CreateTranslation(data.m_cameraPos);
	Math::Matrix rotatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(data.m_cameraDeg.x));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(data.m_cameraDeg.y));
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

	//カメラデータ

	//サウンド再生(仮)
	SOUNDMGR.Play(SoundName::BGM_Ingame);
}
