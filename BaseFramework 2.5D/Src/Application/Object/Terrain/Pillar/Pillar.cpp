#include "Pillar.h"
#include "../../../Scene/SceneManager.h"

Pillar::Pillar(Math::Vector3 a_startPos)
{
	Init();
	m_pos = a_startPos;
}

void Pillar::Update()
{
	//スクロール速度取得
	float speedMulti = SCENEMGR.GetScrollSpeedMulti();

	//角度更新
	m_angleDeg += TERRAINBASEROTATY * speedMulti;
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	//Y座標変化
	m_pos.y += TERRAINBASEMOVEY * speedMulti;

	//Y座標が最大・最小を超えたらリスポーン準備
	if (m_pos.y < PILLARWALLPOSY_MIN)
	{
		m_isDisappear = true;
		m_respawnDir = RespawnDir::Up;
	}
	else if (m_pos.y > PILLARWALLPOSY_MAX)
	{
		m_isDisappear = true;
		m_respawnDir = RespawnDir::Down;
	}
}

void Pillar::DrawLit()
{
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = rotatY * trans;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void Pillar::Respawn()
{
	m_pos.y += PILLARWALLRESPAWNY * (int)m_respawnDir;
	m_isDisappear = false;
}

void Pillar::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/StonePillar/StonePillar.gltf");

	m_angleDeg = 0.0f;
}

void Pillar::Release()
{}
