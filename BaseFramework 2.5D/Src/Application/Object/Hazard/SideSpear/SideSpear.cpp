#include "SideSpear.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Const/StageConst.h"

SideSpear::SideSpear(float a_baseStartPosY, float a_startDeg)
{
	Init();
	m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * BASELINEPOS,a_baseStartPosY + SPAWNPOSY,cosf(DirectX::XMConvertToRadians(m_angleDeg)) * BASELINEPOS };
	m_angleDeg = a_startDeg;
}

void SideSpear::Update()
{
	//スクロール速度取得
	float speedMulti = SCENEMGR.GetScrollSpeedMulti();

	//角度更新
	m_angleDeg += TERRAINBASEROTATY * speedMulti;
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	//角度に合わせて位置変更
	m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * BASELINEPOS,m_pos.y += TERRAINBASEMOVEY * speedMulti,cosf(DirectX::XMConvertToRadians(m_angleDeg)) * BASELINEPOS };
	//一定位置まで下りたら槍が生える
	if (m_pos.y < SPEARACTIVEY)
	{
		m_isSpear = true;
		m_pCollider->SetEnableAll(true);
	}

	//槍移動
	if (m_isSpear)
	{
		if (m_spearPos < SPEARPOSMAX)
		{
			m_spearPos += SPEARMOVEY;
			if (m_spearPos >= SPEARPOSMAX)m_spearPos = SPEARPOSMAX;
		}
	}

	//槍
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos + Math::Vector3(sinf(DirectX::XMConvertToRadians(m_angleDeg)) * m_spearPos, 0, cosf(DirectX::XMConvertToRadians(m_angleDeg)) * m_spearPos));
	m_mWorld = rotatY * trans;
}

void SideSpear::PostUpdate()
{
	float backDeg = SCENEMGR.GetScrollBack();

	//角度更新
	m_angleDeg -= backDeg;
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	//角度に合わせて位置変更
	m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * BASELINEPOS,m_pos.y - TERRAINBASEMOVEY * backDeg,cosf(DirectX::XMConvertToRadians(m_angleDeg)) * BASELINEPOS };

	//消去
	if (m_pos.y < -8.0f)m_isExpired = true;
	//影
	m_isShadow = m_pos.y < SHADOWDRAWSTART ? true : false;

	//穴
	Math::Matrix rotatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mHoleWorld = rotatX * rotatY * trans;

	//槍
	trans = Math::Matrix::CreateTranslation(m_pos + Math::Vector3(sinf(DirectX::XMConvertToRadians(m_angleDeg)) * m_spearPos, 0, cosf(DirectX::XMConvertToRadians(m_angleDeg)) * m_spearPos));
	m_mWorld = rotatY * trans;
}

void SideSpear::DrawLit()
{
	//穴
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_holeModel, m_mHoleWorld);

	//槍（出現済み）
	if (m_isSpear)KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spearModel, m_mWorld);
}

void SideSpear::GenerateDepthMapFromLight()
{
	if (!m_isShadow)return;

	//穴
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_holeModel, m_mHoleWorld);

	//槍（出現済み）
	if (m_isSpear)KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spearModel, m_mWorld);
}

void SideSpear::Init()
{
	m_holeModel = std::make_shared<KdModelData>();
	m_holeModel->Load("Asset/Models/SpearHole/SpearHole.gltf");

	m_spearModel = std::make_shared<KdModelData>();
	m_spearModel->Load("Asset/Models/SideSpear/SideSpear.gltf");

	//当たり判定用意
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("SideSpearHit", m_spearModel, KdCollider::Type::TypeDamage);
	m_pCollider->SetEnableAll(false);
}

void SideSpear::Release()
{}
