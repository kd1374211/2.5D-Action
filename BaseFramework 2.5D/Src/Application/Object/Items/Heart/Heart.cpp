#include "Heart.h"
#include "../../../Scene/SceneManager.h"

Heart::Heart(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos)
{
	Init();

	m_pos = a_baseStartPos + SPAWNPOS;
	m_angleDeg = a_startDeg;
	m_linePos = a_linePos;
}

void Heart::Update()
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

void Heart::PostUpdate()
{
	float backDeg = SCENEMGR.GetScrollBack();

	//角度更新
	m_angleDeg -= backDeg;
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	//角度に合わせて位置変更
	m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos,m_pos.y - TERRAINBASEMOVEY * backDeg,cosf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos };

	//Y座標が一定未満なら消去
	if (m_pos.y < -8.0f)m_isExpired = true;
	//影
	m_isShadow = m_pos.y < SHADOWDRAWSTART ? true : false;

	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(210.0f));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = rotatY * trans;
}

void Heart::PreDraw()
{
	m_polygon->SetUVRect((int)m_animCnt);
}

void Heart::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld);
}

void Heart::GenerateDepthMapFromLight()
{
	if (!m_isShadow)return;
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld);
}

void Heart::Init()
{
	//ポリゴン
	m_polygon = std::make_shared<KdSquarePolygon>();
	m_polygon->SetMaterial("Asset/Textures/Items/Heart.png");
	m_polygon->SetSplit(12, 1);
	m_polygon->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);

	//当たり判定
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Heart", SPHEREHITOFS, 0.5f, KdCollider::TypeHeal);
}

void Heart::Release()
{}

void Heart::UpdateAnim()
{
	m_animCnt += ANIMADD;
	if (m_animCnt >= ANIMMAX)m_animCnt -= ANIMMAX;
}
