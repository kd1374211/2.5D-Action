#include "Wall.h"
#include "../../../Scene/SceneManager.h"

Wall::Wall(Math::Vector3 a_startPos)
{
	Init();
	m_pos = a_startPos;
}

void Wall::Update()
{
	m_pos.y -= 0.05f;
	if (m_pos.y < -192.0f)
	{
		m_pos.y += 384.0f;
	}
	SetPos(m_pos);
}

void Wall::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void Wall::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void Wall::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Wall/Wall.gltf");

	SetScale(2.0f);
}

void Wall::Release()
{}
