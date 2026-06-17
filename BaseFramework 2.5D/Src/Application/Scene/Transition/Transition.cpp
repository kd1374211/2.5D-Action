#include "Transition.h"
#include "../SceneManager.h"

void Transition::Update()
{
	m_pos.y -= 60.0f;
	if (m_pos.y <= 0.0f && !m_isReset)m_isReset = true;

	if (m_pos.y <= -1200.0f)
	{
		m_isExpired = true;
	}
}

void Transition::DrawSprite()
{
	Math::Rectangle rec = Math::Rectangle(0, 0, 128, 72);

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, m_pos.x, m_pos.y, 1280.0f, 1200.0f, &rec);
}

void Transition::Init()
{
	m_tex = std::make_shared<KdTexture>();
	m_tex->Load("Asset/Textures/Transition/BlackBack.png");
	m_pos = Math::Vector3(0, 1200.0f, 0);
}

void Transition::Release()
{}
