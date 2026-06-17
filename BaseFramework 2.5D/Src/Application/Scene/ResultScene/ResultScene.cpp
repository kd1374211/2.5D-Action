#include "ResultScene.h"
#include "../../StageSpawner/StageSpawner.h"
#include "../../Object/Character/CharaManager.h"
#include "../SceneManager.h"
#include "../../Object/Sprite/Scene/ResultWindow/ResultWindow.h"

void ResultScene::Init()
{
	m_camera = std::make_shared<KdCamera>();
	m_camera->SetProjectionMatrix(90);

	//地形生成
	STAGESPAWNER.StartGame(this);

	//リザルトウィンドウ
	std::shared_ptr<ResultWindow> window = std::make_shared<ResultWindow>();
	m_wpResultWindow = window;
	AddObject(window);

	//スクロール
	SCENEMGR.SetScrollSpeedMulti();

	//カメラ
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
}

void ResultScene::Event()
{
	//リザルト終了になるまで数える
	if (!m_isResultEnd)m_countF++;

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
				m_canResultEnd = true;
			}

			if (m_countF >= RESULTENDF)m_canResultEnd = true;
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
					}
				}
			}
		}
	}

	//スペースキー
	m_isSpaceKey = GetAsyncKeyState(VK_SPACE) & 0x8000;

	//地形更新
	STAGESPAWNER.Update();

	Math::Matrix trans = Math::Matrix::CreateTranslation(m_cameraPos);
	Math::Matrix rotatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_cameraDeg.x));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_cameraDeg.y));
	Math::Matrix rotatY2 = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_transitionScroll));
	Math::Matrix mat = rotatY * rotatX * trans * rotatY2;
	m_camera->SetCameraMatrix(mat);
}
