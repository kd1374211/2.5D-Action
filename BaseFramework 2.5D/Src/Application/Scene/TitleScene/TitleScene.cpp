#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../StageSpawner/StageSpawner.h"
#include "../../Object/Character/CharaManager.h"
#include "../../Object/Sprite/Scene/TitleObject/TitleObject.h"
#include "../../Sound/SoundManager.h"
#include "../../Camera/CameraManager.h"

void TitleScene::Event()
{
	if (m_isFadeIn)
	{
		//フェードイン処理
		if (!m_wpTitleObject.expired())
		{
			m_titleVolumeMulti += BGMFADESPEED;

			if (m_wpTitleObject.lock()->GetIsFadeInEnd())
			{
				m_isFadeIn = false;
				m_titleVolumeMulti = 1.0f;
			}

			if (m_titleVolumeMulti > 1.0f)m_titleVolumeMulti = 1.0f;
		}
	}
	else
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !m_isSpaceKey && !m_isFadeStart)
		{
			// タイトルオブジェクトのフェードアウト処理
			if (!m_wpTitleObject.expired())
			{
				m_wpTitleObject.lock()->SetFadeFlg(true);
				m_isFadeStart = true;
				SOUNDMGR.Play(SoundName::SE_GameStart);
			}
		}

		if (m_isFadeStart)
		{
			//消滅確認
			if (!m_wpTitleObject.expired())
			{
				if (m_wpTitleObject.lock()->GetIsFadeEnd())
				{
					SceneManager::Instance().SetNextScene
					(
						SceneManager::SceneType::Game
					);

					SOUNDMGR.Stop(SoundName::BGM_Title);
				}

				m_scrollSpeed += SCROLLADD_AFTERSTART;
				m_titleVolumeMulti -= BGMFADESPEED;
				if (m_titleVolumeMulti < 0.0f)m_titleVolumeMulti = 0.0f;
			}
		}
	}

	m_isSpaceKey = (GetAsyncKeyState(VK_SPACE) & 0x8000);

	//キャラ更新
	CHARAMGR.Update();

	//地形更新
	STAGESPAWNER.Update();

	//音量更新
	SOUNDMGR.VolumeChange(SoundName::BGM_Title, m_titleVolumeMulti);

	//スクロール速度設定
	SCENEMGR.SetScrollSpeedMulti(m_scrollSpeed);
}

TitleScene::TitleScene(bool a_isFadeIn)
{
	m_isFadeIn = a_isFadeIn;
	
	Init();
}

void TitleScene::Init()
{
	m_camera = std::make_shared<KdCamera>();
	m_camera->SetProjectionMatrix(80);

	//タイトル画像関連
	std::shared_ptr<TitleObject> titleObj = std::make_shared<TitleObject>(m_isFadeIn);
	m_wpTitleObject = titleObj;
	AddObject(titleObj);

	//曲再生
	SOUNDMGR.Play(SoundName::BGM_Title);
	//最初以外ならフェードイン
	m_titleVolumeMulti = m_isFadeIn ? 0.2f : 1.0f;
	SOUNDMGR.VolumeChange(SoundName::BGM_Title, m_titleVolumeMulti);

	//プレイヤー（一応リセット）
	CHARAMGR.SpawnPlayer(this);

	//地形生成
	STAGESPAWNER.StartGame(this);

	//カメラ
	CameraBaseData data = CAMERAMGR.GetCameraData();

	Math::Matrix trans = Math::Matrix::CreateTranslation(data.m_cameraPos);
	Math::Matrix rotatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(data.m_cameraDeg.x));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(data.m_cameraDeg.y));
	Math::Matrix mat = rotatY * rotatX * trans;
	m_camera->SetCameraMatrix(mat);
}
