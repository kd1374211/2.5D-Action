#include "ResultScene.h"
#include "../../StageSpawner/StageSpawner.h"
#include "../../Object/Character/CharaManager.h"
#include "../SceneManager.h"
#include "../../Object/Sprite/Scene/ResultWindow/ResultWindow.h"
#include "../../Object/Sprite/Scene/ResultStars/ResultStars.h"
#include "../../Sound/SoundManager.h"
#include "../../Camera/CameraManager.h"

void ResultScene::Init()
{
	m_camera = std::make_shared<KdCamera>();
	m_camera->SetProjectionMatrix(80);

	//地形生成
	STAGESPAWNER.StartGame(this);

	//リザルトウィンドウ
	std::shared_ptr<ResultWindow> window = std::make_shared<ResultWindow>();
	m_wpResultWindow = window;
	AddObject(window);

	//その星部分
	std::shared_ptr<ResultStars>star = std::make_shared<ResultStars>();
	star->SetOwner(window);
	AddObject(star);
}

void ResultScene::Event()
{
	//スクロールリセット
	SCENEMGR.SetScrollSpeedMulti(1.0f);

	if (!m_canResultEnd)
	{
		//カウント
		m_countF++;

		//効果音（ジャン）
		if (m_countF == RANDDRAWSTARTF)SOUNDMGR.Play(SoundName::SE_ResultRoll);
		else if (m_countF == HEIGHTDRAWF || m_countF == KILLSDRAWF)SOUNDMGR.Play(SoundName::SE_RollEnd);
		else if (m_countF == RANKTEXTDRAWF && !m_isRankEndSE)
		{
			SOUNDMGR.Stop(SoundName::SE_ResultRoll);
			SOUNDMGR.Play(SoundName::SE_RollEnd);
			m_isRankEndSE = true;
		}
	}

	//リザルト開始前
	if (!m_isResultStart)
	{
		if (m_countF >= RESULTSTARTF)m_isResultStart = true;
	}
	//リザルト開始後
	else
	{
		if (!m_canResultEnd)
		{
			//リザルトスキップ
			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !m_isSpaceKey)
			{
				m_countF = RESULTENDF;
				SOUNDMGR.Stop(SoundName::SE_ResultRoll);
				if (!m_isRankEndSE)SOUNDMGR.Play(SoundName::SE_RollEnd);
			}

			//カウントセット
			if (!m_wpResultWindow.expired())m_wpResultWindow.lock()->SetCountF(m_countF);

			//リザルト終了可能かフラグ
			if (m_countF >= RESULTENDF)
			{
				m_canResultEnd = true;
				SOUNDMGR.Play(SoundName::BGM_Result);
			}
		}
		//リザルト終了（操作可能）
		else
		{
			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !m_isSpaceKey && !m_isResultEnd)
			{
				if (!m_wpResultWindow.expired())
				{
					m_wpResultWindow.lock()->ResultEnd();
					m_isResultEnd = true;
					SOUNDMGR.Play(SoundName::SE_GameStart);
				}
			}

			if (m_isResultEnd)
			{
				if (!m_wpResultWindow.expired())
				{
					if(m_wpResultWindow.lock()->GetIsWindowOut())
					{
						m_isTitleTransition = true;
					}
				}

				if (m_isTitleTransition)
				{
					//タイトルへのスクロール
					if (m_scrollSpeed < TRANSITIONSCROLLMAX && !m_isTitleTransitionStop)
					{
						m_scrollSpeed += TRANSITIONSCROLLADD;
						if (m_scrollSpeed >= TRANSITIONSCROLLMAX)
						{
							m_scrollSpeed = TRANSITIONSCROLLMAX;
							m_isTitleTransitionStop = true;
							m_transitionScroll += 360.0f;
						}
					}
					else if (m_isTitleTransitionStop)
					{
						if (m_transitionScroll <= 180.0f && !m_isPlayerSpawned)
						{
							CHARAMGR.ResetPlayer();
							CHARAMGR.SpawnPlayer();
							m_isPlayerSpawned = true;
						}
						else if (m_isPlayerSpawned)
						{
							m_scrollSpeed = m_transitionScroll / 12.0f;
							if (m_scrollSpeed < 1.0f)m_scrollSpeed = 1.0f;
							else if (m_scrollSpeed > 10.0f)m_scrollSpeed = 10.0f;
						}
					}

					//角度更新
					m_transitionScroll -= m_scrollSpeed;
					if (m_transitionScroll < 0.0f)m_transitionScroll += 360.0f;

					//タイトル移行
					if (m_isPlayerSpawned && m_transitionScroll < 1.0f)
					{
						SceneManager::Instance().SetNextScene
						(
							SceneManager::SceneType::Title
						);

						SOUNDMGR.Stop(SoundName::BGM_Result);
					}
				}

				//音量変化
				m_resultVolumeMulti -= BGMFADESPEED;
				if (m_resultVolumeMulti < 0.0f)m_resultVolumeMulti = 0.0f;

				//セット
				SOUNDMGR.VolumeChange(SoundName::BGM_Result, m_resultVolumeMulti);
			}
		}
	}

	//スペースキー
	m_isSpaceKey = GetAsyncKeyState(VK_SPACE) & 0x8000;

	//地形更新
	STAGESPAWNER.Update();

	//カメラ
	CameraBaseData data = CAMERAMGR.GetCameraData();

	Math::Matrix trans = Math::Matrix::CreateTranslation(data.m_cameraPos);
	Math::Matrix rotatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(data.m_cameraDeg.x));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(data.m_cameraDeg.y));
	Math::Matrix rotatY2 = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_transitionScroll));
	Math::Matrix mat = rotatY * rotatX * trans * rotatY2;
	m_camera->SetCameraMatrix(mat);
}
