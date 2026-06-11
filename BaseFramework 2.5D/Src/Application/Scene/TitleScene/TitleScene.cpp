#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../StageSpawner/StageSpawner.h"
#include "../../Object/Character/CharaManager.h"

void TitleScene::Event()
{
	if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState('S') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Game
		);
	}

	//キャラ更新
	CHARAMGR.Update();

	//地形更新
	STAGESPAWNER.Update();

	Math::Matrix trans = Math::Matrix::CreateTranslation(m_cameraPos);
	Math::Matrix rotatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_cameraDeg.x));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_cameraDeg.y));
	Math::Matrix mat = rotatY * rotatX * trans;
	m_camera->SetCameraMatrix(mat);
}

void TitleScene::Init()
{
	m_camera = std::make_shared<KdCamera>();
	m_camera->SetProjectionMatrix(90);

	//地形生成
	STAGESPAWNER.StartGame(this);

	//プレイヤー
	CHARAMGR.StartGame(this);

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
