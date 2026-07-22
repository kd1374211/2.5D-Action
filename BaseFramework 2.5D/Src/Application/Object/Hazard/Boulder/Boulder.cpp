#include "Boulder.h"
#include "../../../Scene/SceneManager.h"

Boulder::Boulder(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos)
{
	Init();
	m_pos = a_baseStartPos + SPAWNPOS;
	m_angleDeg = a_startDeg;
	m_linePos = a_linePos;
}

void Boulder::Update()
{
	//アルファリセット
	SetAlpha(1.0f);

	//スクロール速度取得
	float speedMulti = SCENEMGR.GetScrollSpeedMulti();

	//角度更新
	m_angleDeg += TERRAINBASEROTATY * (speedMulti + SPEEDMULTIADD);
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;
	
	//角度に合わせて位置変更
	m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos,m_pos.y += TERRAINBASEMOVEY * (speedMulti + SPEEDMULTIADD),cosf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos};

	//回転
	m_angleDegZ -= 2.0f;
	if (m_angleDegZ < 0.0f)m_angleDeg += 360.0f;

	Math::Matrix rotatZ = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angleDegZ));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = rotatZ * rotatY * trans;
}

void Boulder::PostUpdate()
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
	m_isShadow = m_pos.y < SHADOWDRAWSTART + SPAWNPOS.y ? true : false;

	Math::Matrix rotatZ = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angleDegZ));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = rotatZ * rotatY * trans;
}

void Boulder::DrawLit()
{
	Math::Color color = Math::Color(1.0f, 1.0f, 1.0f, m_alpha);
	if (m_alpha < 1.0f)KdShaderManager::Instance().ChangeDepthStencilState(KdDepthStencilState::ZWriteDisable);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, color);
	if (m_alpha < 1.0f)KdShaderManager::Instance().ChangeDepthStencilState(KdDepthStencilState::ZEnable);
}

void Boulder::GenerateDepthMapFromLight()
{
	if (!m_isShadow)return;
	Math::Color color = Math::Color(1.0f, 1.0f, 1.0f, m_alpha);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, color);
}

void Boulder::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Boulder/Boulder.gltf");

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Boulder", m_model, KdCollider::Type::TypeDamage | KdCollider::Type::TypeHide | KdCollider::Type::TypeRollingObj);
}

void Boulder::Release()
{}
