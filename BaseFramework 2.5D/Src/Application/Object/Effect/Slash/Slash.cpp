#include "Slash.h"
#include "../../Character/CharacterInclude.h"

Slash::Slash(Math::Vector3 a_startPos, float a_scale, float a_angle)
{
	Init();
	m_pos = a_startPos;
	m_scale = a_scale;
	m_angleDeg = a_angle;

	m_polygon->SetScale(1.2f);
}

void Slash::Update()
{
	m_polygon->SetUVRect((int)m_animCnt);
	//アニメーション更新
	m_animCnt += ANIMADD;
	if (m_animCnt >= ANIMMAX)
	{
		m_animCnt = ANIMMAX;
		m_isExpired = true;
	}

	if (m_animCnt >= HITEND)m_pCollider->SetEnableAll(false);

	//マトリックス
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	m_mWorld = rotatY * trans;
}

void Slash::DrawEffect()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld);
}

void Slash::Init()
{
	//ポリゴン
	m_polygon = std::make_shared<KdSquarePolygon>();
	m_polygon->SetMaterial("Asset/Textures/Effect/Slash.png");
	m_polygon->SetSplit(8, 1);
	
	//当たり判定
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerAttack", Math::Vector3::Zero, 0.4f, KdCollider::TypeDamage_Enemy);
}

void Slash::Release()
{}
