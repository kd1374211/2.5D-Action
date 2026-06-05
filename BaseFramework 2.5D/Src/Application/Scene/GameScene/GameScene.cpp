#include "GameScene.h"
#include"../SceneManager.h"
#include "../../Object/Terrain/Pillar/Pillar.h"
#include "../../Object/Terrain/Wall/Wall.h"
#include "../../Object/Terrain/Stair/Stair.h"
#include "../../Object/Character/Player/Player.h"
#include "../../StageSpawner/StageSpawner.h"
#include "../../Object/Sprite/Number/HeightsNumber.h"

void GameScene::Event()
{
	if (GetAsyncKeyState('T') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}

	//ステージアップデート
	STAGESPAWNER.Update();

	if (GetAsyncKeyState('Z') & 0x8000)
	{
		m_cameraPos.x += 0.2f;
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		m_cameraPos.x -= 0.2f;
	}
	if (GetAsyncKeyState('C') & 0x8000)
	{
		m_cameraPos.y += 0.2f;
	}
	if (GetAsyncKeyState('V') & 0x8000)
	{
		m_cameraPos.y -= 0.2f;
	}
	if (GetAsyncKeyState('B') & 0x8000)
	{
		m_cameraPos.z += 0.2f;
	}
	if (GetAsyncKeyState('N') & 0x8000)
	{
		m_cameraPos.z -= 0.2f;
	}
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
	AddObject(std::make_shared<Player>());

	//ステージスポーナーに任せた
	STAGESPAWNER.StartGame(this);

	//現在の高さ
	AddObject(std::make_shared<HeightsNumber>());

	m_camera = std::make_unique<KdCamera>();
	m_camera->SetProjectionMatrix(60);
	m_camera->SetCameraMatrix(Math::Matrix::CreateTranslation(0.0f, 5.0f, -45.0f));

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
}
