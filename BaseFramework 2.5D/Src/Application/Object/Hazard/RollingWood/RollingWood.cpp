#include "RollingWood.h"
#include "../../../Scene/SceneManager.h"

RollingWood::RollingWood(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos, float a_length)
{
	Init();
	m_pos = a_baseStartPos + SPAWNPOS;
	m_angleDeg = a_startDeg;
	m_linePos = a_linePos;
	m_length = a_length;
}

void RollingWood::Update()
{
	//スクロール速度取得
	float speedMulti = SCENEMGR.GetScrollSpeedMulti();

	//角度更新
	m_angleDeg += TERRAINBASEROTATY * (speedMulti + SPEEDMULTIADD);
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	//角度に合わせて位置変更
	m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos,m_pos.y += TERRAINBASEMOVEY * (speedMulti + SPEEDMULTIADD),cosf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos };

	//回転
	m_angleDegZ -= 2.0f;
	if (m_angleDegZ < 0.0f)m_angleDeg += 360.0f;

	Math::Matrix scale = Math::Matrix::CreateScale(Math::Vector3(1, 1, m_length));
	Math::Matrix rotatZ = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angleDegZ));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = scale * rotatZ * rotatY * trans;
}

void RollingWood::PostUpdate()
{
	float backDeg = SCENEMGR.GetScrollBack();

	//角度更新
	m_angleDeg -= backDeg;
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	//角度に合わせて位置変更
	m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos,m_pos.y - TERRAINBASEMOVEY * backDeg,cosf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos };

	//消滅
	if (m_pos.y < -8.0f)m_isExpired = true;
	//影
	m_isShadow = m_pos.y < SHADOWDRAWSTART ? true : false;

	Math::Matrix scale = Math::Matrix::CreateScale(Math::Vector3(1, 1, m_length));
	Math::Matrix rotatZ = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angleDegZ));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = scale * rotatZ * rotatY * trans;
}

void RollingWood::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void RollingWood::GenerateDepthMapFromLight()
{
	if (!m_isShadow)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
void RollingWood::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/RollingWood/RollingWood.gltf");

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Wood", m_model, KdCollider::Type::TypeDamage | KdCollider::Type::TypeRollingObj);
}

void RollingWood::Release()
{}
