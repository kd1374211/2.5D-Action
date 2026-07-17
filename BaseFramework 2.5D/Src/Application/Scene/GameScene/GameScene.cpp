#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Object/Character/Player/Player.h"
#include "../../Object/Character/CharaManager.h"
#include "../../StageSpawner/StageSpawner.h"
#include "../../Object/Sprite/HeightsNumber/HeightsNumber.h"
#include "../../Object/Sprite/KillCount/KillCount.h"
#include "../Transition/Transition.h"
#include "../../Sound/SoundManager.h"
#include "../../Camera/CameraManager.h"
#include "../../Object/Sprite/Scene/KeyGuide/KeyGuide.h"
#include "../../Object/Sprite/Scene/HeartChargeBar/HeartChargeBar.h"
#include "../../HeartCharge/HeartCharge.h"

void GameScene::Event()
{
	//ハートチャージ更新
	HEARTCHARGE.Update();

	//ステージ更新
	STAGESPAWNER.Update();

	//カメラシェイク
	CAMERAMGR.UpdateScreenShake();

	//ゲーム開始前
	if (!SCENEMGR.GetGameStartFlg())
	{
		//待機
		if (m_countF_start < STARTF)m_countF_start++;

		//フェードイン
		if (m_countF_start >= FADEINF && !m_isFadeIn)
		{
			//現在の高さ
			AddObject(std::make_shared<HeightsNumber>());

			//キルカウント
			AddObject(std::make_shared<KillCount>());

			//操作ガイド
			AddObject(std::make_shared<KeyGuide>());

			//ハート
			CHARAMGR.GetPlayer()->SetIsHeartTex(true);

			//ハートチャージ
			AddObject(std::make_shared<HeartChargeBar>());

			m_isFadeIn = true;
		}
		else if (m_countF_start >= STARTF)
		{
			//サウンド再生
			SOUNDMGR.Play(SoundName::BGM_Ingame);

			//フラグセット
			SCENEMGR.SetGameStartFlg(true);
			SCOREMGR.SetCountStart(true);
		}

		//セット
		SCENEMGR.SetScrollSpeedMulti(1.0f);
	}
	else if (!m_wpPlayer.expired())
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

					//ゲーム開始フラグ停止
					SCENEMGR.SetGameStartFlg(false);

					//スクリーンシェイク停止
					CAMERAMGR.SetCameraShakeFlg(false);
				}
			}
		}
	}

	//データ回収
	CameraBaseData data = CAMERAMGR.GetCameraData();
	Math::Vector3 localPos = CAMERAMGR.GetLocalCameraPos();

	m_camera->SetProjectionMatrix(data.m_viewAngle);
	Math::Matrix trans = Math::Matrix::CreateTranslation(data.m_cameraPos + localPos);
	Math::Matrix rotatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(data.m_cameraDeg.x));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(data.m_cameraDeg.y));
	Math::Matrix mat = rotatY * rotatX * trans;
	m_camera->SetCameraMatrix(mat);
}

void GameScene::PreDraw()
{
	//隠し処理
	CAMERAMGR.PreDraw(m_camera);

	// カメラ情報がある場合はシェーダーにセット
	if (m_camera)
	{
		m_camera->SetToShader();
	}

	for (auto& obj : m_objList)
	{
		obj->PreDraw();
	}
}

void GameScene::Init()
{
	m_camera = std::make_shared<KdCamera>();
	m_camera->SetProjectionMatrix(80);
	
	//プレイヤー
	CHARAMGR.SpawnPlayer(this);
	m_wpPlayer = CHARAMGR.GetPlayer();

	//ステージ
	STAGESPAWNER.StartGame(this);

	//スコアリセット
	SCOREMGR.Reset();

	//チャージリセット
	HEARTCHARGE.Reset();
}
