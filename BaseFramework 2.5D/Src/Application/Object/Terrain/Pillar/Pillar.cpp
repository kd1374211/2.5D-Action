#include "Pillar.h"
#include "../../../Scene/SceneManager.h"

Pillar::Pillar(Math::Vector3 a_startPos)
{
	Init();
	m_pos = a_startPos;
}

void Pillar::Update()
{
	m_pos.y -= 0.05f;
	if (m_pos.y < -192.0f)
	{
		m_pos.y += 384.0f;
	}
	SetPos(m_pos);
}

void Pillar::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void Pillar::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void Pillar::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/StonePillar/StonePillar.gltf");

	SetScale(2.0f);
}

void Pillar::Release()
{}
