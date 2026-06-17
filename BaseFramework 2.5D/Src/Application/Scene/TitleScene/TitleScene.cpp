#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../StageSpawner/StageSpawner.h"
#include "../../Object/Character/CharaManager.h"
#include "../../Object/Sprite/Scene/TitleObject/TitleObject.h"

void TitleScene::Event()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !m_isSpaceKey && !m_isFadeStart)
	{
		// タイトルオブジェクトのフェードアウト処理
		if (!m_wpTitleObject.expired())
		{
			m_wpTitleObject.lock()->SetFadeFlg(true);
			m_isFadeStart = true;
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
			}
		}
	}

	m_isSpaceKey = (GetAsyncKeyState(VK_SPACE) & 0x8000);

	//キャラ更新
	CHARAMGR.Update();

	//地形更新
	STAGESPAWNER.Update();
}

void TitleScene::Init()
{
	m_camera = std::make_shared<KdCamera>();
	m_camera->SetProjectionMatrix(90);

	//タイトル画像関連
	std::shared_ptr<TitleObject> titleObj = std::make_shared<TitleObject>();
	m_wpTitleObject = titleObj;
	AddObject(titleObj);

	//プレイヤー（一応リセット）
	CHARAMGR.SpawnPlayer(this);

	//地形生成
	STAGESPAWNER.StartGame(this);

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

	Math::Matrix trans = Math::Matrix::CreateTranslation(m_cameraPos);
	Math::Matrix rotatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_cameraDeg.x));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_cameraDeg.y));
	Math::Matrix mat = rotatY * rotatX * trans;
	m_camera->SetCameraMatrix(mat);
}
