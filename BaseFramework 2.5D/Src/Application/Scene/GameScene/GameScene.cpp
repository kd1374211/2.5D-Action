#include "GameScene.h"
#include"../SceneManager.h"
#include "../../Object/Terrain/Pillar/Pillar.h"
#include "../../Object/Terrain/Wall/Wall.h"
#include "../../Object/Terrain/Stair/Stair.h"
#include "../../Object/Character/Player/Player.h"

void GameScene::Event()
{
	if (GetAsyncKeyState('T') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}

	m_countF++;
	if (m_countF >= 160)
	{
		m_countF = 0;
	}

	//カメラ
	m_cameraDeg -= 0.3f;
	if (m_cameraDeg > 360.0f)m_cameraDeg -= 360.0f;
	else if (m_cameraDeg < 0.0f)m_cameraDeg += 360.0f;

	Math::Matrix trans = Math::Matrix::CreateTranslation(0, -15.0f, -90.0f);
	Math::Matrix rotatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(5));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(5));
	Math::Matrix rotatY2 = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_cameraDeg));
	Math::Matrix mat = rotatY * rotatX * trans * rotatY2;
	m_camera->SetCameraMatrix(mat);
}

void GameScene::Init()
{
	m_countF = 0;
	m_cameraDeg = 0.0f;

	AddObject(std::make_shared<Player>(Math::Vector3(0.0f, 0.0f, 0.0f)));

	for (int i = 0; i < 3; i++)
	{
		float posY = -128.0f + (float)i * 128.0f;
		AddObject(std::make_shared<Pillar>(Math::Vector3(0.0f, posY, 0.0f)));
		AddObject(std::make_shared<Wall>(Math::Vector3(0.0f, posY, 0.0f)));
	}

	for (int i = 0; i < 120; i++)
	{
		float angleDeg = -(float)i * 6.0f;
		Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(angleDeg)) * 40.0f,-60.0f + (float)i,cosf(DirectX::XMConvertToRadians(angleDeg)) * 40.0f };
		AddObject(std::make_shared<Stair>(pos, angleDeg));
	}

	m_camera = std::make_unique<KdCamera>();
	m_camera->SetProjectionMatrix(60);
	m_camera->SetCameraMatrix(Math::Matrix::CreateTranslation(0, -15.0f, -90.0f));
}
