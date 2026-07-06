#include "SlashHit.h"
#include "../../Character/Enemy/EnemyBase.h"

SlashHit::SlashHit(std::shared_ptr<EnemyBase> a_target)
{
	Init();
	m_wpTarget = a_target;
	m_angleDeg = 210.0f;
}

void SlashHit::Update()
{
	m_polygon->SetUVRect((int)m_animCnt);
	//アニメーション更新
	m_animCnt += ANIMADD;
	if (m_animCnt >= ANIMMAX)
	{
		m_animCnt = ANIMMAX;
		m_isExpired = true;
	}
}

void SlashHit::PostUpdate()
{
	//位置更新
	if (m_wpTarget.expired())return;

	//ターゲットを取得
	std::shared_ptr<EnemyBase> target = m_wpTarget.lock();

	//情報取得
	Math::Vector3 pos = target->GetPos() + target->GetHitOfsPos();
	
	//マトリックス
	Math::Matrix trans = Math::Matrix::CreateTranslation(pos);
	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleDeg));
	m_mWorld = rotatY * trans;
}

void SlashHit::DrawEffect()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld);
}

void SlashHit::Init()
{
	m_polygon = std::make_shared<KdSquarePolygon>();
	m_polygon->SetMaterial("Asset/Textures/Effect/SlashHit.png");
	m_polygon->SetSplit(6, 2);
	m_polygon->SetScale(1.5f);
}

void SlashHit::Release()
{}
