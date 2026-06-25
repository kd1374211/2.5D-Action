#include "Heal.h"
#include "../../Character/CharaManager.h"
#include "../../Character/Player/Player.h"

void Heal::Update()
{
	UpdateAnim();

	//移動
	m_posDiffY += POSDIFFMOVE;
	
	//透明度
	m_alpha += ALPHACHANGE;
	if (m_alpha <= 0.0f)m_alpha = 0.0f;
}

void Heal::PostUpdate()
{
	//プレイヤー座標に合わせる
	Math::Vector3 playerPos = CHARAMGR.GetPlayer()->GetPos();
	m_pos = playerPos + BASEPOSDIFF + Math::Vector3(0, m_posDiffY, 0);

	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = trans;
}

void Heal::DrawBright()
{
	Math::Color color = Math::Color(1.0f, 1.0f, 1.0f, m_alpha);
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld, color);
}

void Heal::DrawEffect()
{
	Math::Color color = Math::Color(1.0f, 1.0f, 1.0f, m_alpha);
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld, color);
}

void Heal::Init()
{
	m_polygon = std::make_shared<KdSquarePolygon>();
	m_polygon->SetMaterial("Asset/Textures/Effect/Heal.png");
	m_polygon->SetSplit(ANIMSPLIT_X, ANIMSPLIT_Y);
	m_polygon->SetScale(0.8f);
}

void Heal::Release()
{}

void Heal::UpdateAnim()
{
	m_animCnt += ANIMADD;
	if (m_animCnt >= ANIMMAX)
	{
		m_isExpired = true;
	}
	m_polygon->SetUVRect((int)m_animCnt);
}
