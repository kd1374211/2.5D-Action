#include "Stair.h"
#include "../../../Scene/SceneManager.h"
#include "../../../StageSpawner/StageSpawner.h"

Stair::Stair(Math::Vector3 a_startPos,float a_startDeg)
{
	Init();
	m_pos = a_startPos;
	m_angleDeg = a_startDeg;
	m_mWorld = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
}

void Stair::Update()
{
	float speedMulti = SCENEMGR.GetScrollSpeedMulti();

	//角度更新
	m_angleDeg += TERRAINBASEROTATY * speedMulti;
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	//角度に合わせて位置変更
	if (!test)
	{
		m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * 4.0f,m_pos.y += TERRAINBASEMOVEY * speedMulti,cosf(DirectX::XMConvertToRadians(m_angleDeg)) * 4.0f };
	}
	else
	{
		m_pos = { -250,m_pos.y += TERRAINBASEMOVEY * speedMulti,100 };
	}

	//Y座標が最大・最小を超えたらリスポーン準備
	if (m_pos.y < STAIRPOSY_MIN)
	{
		m_isDisappear = true;
		m_respawnDir = RespawnDir::Up;
	}
	else if (m_pos.y > STAIRPOSY_MAX)
	{
		m_isDisappear = true;
		m_respawnDir = RespawnDir::Down;
	}

	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = rotatY * trans;
}

void Stair::PostUpdate()
{
	float backDeg = SCENEMGR.GetScrollBack();

	//角度更新
	m_angleDeg -= backDeg;
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	//角度に合わせて位置変更
	if (!test)
	{
		m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * 4.0f,m_pos.y - TERRAINBASEMOVEY * backDeg,cosf(DirectX::XMConvertToRadians(m_angleDeg)) * 4.0f };
	}
	else
	{
		m_pos.y -= TERRAINBASEMOVEY * backDeg;
	}

	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = rotatY * trans;
}

void Stair::DrawLit()
{
	if (test)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void Stair::Respawn(bool a_test)
{
	//階段のログを格納
	if (m_respawnDir == RespawnDir::Down)STAGESPAWNER.AddFutureStairVisibleLog(test);
	else STAGESPAWNER.AddPastStairVisibleLog(test);

	test = a_test;

	m_pos.y += STAIRRESPAWNY * (int)m_respawnDir;
	m_isDisappear = false;
}

void Stair::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Stair/Stair.gltf");

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StairCollision", m_model, KdCollider::TypeGround);
}

void Stair::Release()
{}
