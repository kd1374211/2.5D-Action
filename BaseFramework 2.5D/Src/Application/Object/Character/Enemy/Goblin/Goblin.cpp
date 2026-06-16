#include "Goblin.h"
#include "../../../../Scene/SceneManager.h"

Goblin::Goblin(Math::Vector3 a_startPos, float a_startDeg, float a_linePos)
{
	Init();
	m_pos = a_startPos;
	m_angleDeg = a_startDeg;
	m_linePos = a_linePos;
}

void Goblin::Update()
{
	float speedMulti = SCENEMGR.GetScrollSpeedMulti();

	//角度更新
	m_angleDeg += TERRAINBASEROTATY * speedMulti;
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	//角度に合わせて位置変更
	m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos,m_pos.y += TERRAINBASEMOVEY * speedMulti,cosf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos };

	//アニメーション更新
	UpdateAnim();
	
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = trans;
}

void Goblin::Init()
{
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Goblin", { 0,0.6f,0 }, 0.6f, KdCollider::TypeDamage);
}

void Goblin::Release()
{}
