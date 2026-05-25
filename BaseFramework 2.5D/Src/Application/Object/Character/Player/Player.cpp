#include "Player.h"

Player::Player(Math::Vector3 a_startPos)
{
	Init();
	m_pos = a_startPos;
	m_linePos = LINEPOSSTART;
}

void Player::Update()
{
	m_angleDeg -= 0.3f;
	if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_linePos -= LINEMOVE;
		if (m_linePos < LINEPOSMIN)m_linePos = LINEPOSMIN;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_linePos += LINEMOVE;
		if (m_linePos > LINEPOSMAX)m_linePos = LINEPOSMAX;
	}

	m_pos = { 0.0f,-15.0f,-m_linePos };

	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));

	m_mWorld = trans * rotat;
}

void Player::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld);
}

void Player::Init()
{
	m_polygon = std::make_shared<KdSquarePolygon>();
	m_polygon->SetMaterial("Asset/Textures/Chara/Player/PlayerRun.png");
	m_animCnt = 0;
	m_angleDeg = 0.0f;
}

void Player::Release()
{}
