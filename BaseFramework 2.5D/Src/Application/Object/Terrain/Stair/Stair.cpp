#include "Stair.h"
#include "../../../Scene/SceneManager.h"

Stair::Stair(Math::Vector3 a_startPos, float a_startDeg)
{
	Init();
	m_pos = a_startPos;
	m_angleDeg = a_startDeg;
	m_mWorld = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	SetScale(2.0f);
}

void Stair::Update()
{
	m_pos.y -= 0.05f;
	if (m_pos.y < -60.0f)
	{
		Math::Vector3 pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * 40.0f,60.0f,cosf(DirectX::XMConvertToRadians(m_angleDeg)) * 40.0f };
		SCENEMGR.AddObject(std::make_shared<Stair>(pos, m_angleDeg));
		m_isExpired = true;
	}
	SetPos(m_pos);
}

void Stair::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void Stair::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void Stair::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Stair/Stair.gltf");
}

void Stair::Release()
{}
