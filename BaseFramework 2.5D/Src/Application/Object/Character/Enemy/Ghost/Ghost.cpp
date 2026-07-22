#include "Ghost.h"
#include "../../../../Scene/SceneManager.h"

Ghost::Ghost(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos)
{
	Init();
	m_pos = a_baseStartPos + SPAWNPOS;
	m_angleDeg = a_startDeg;
	m_linePos = a_linePos;
}

void Ghost::Update()
{
	float speedMulti = SCENEMGR.GetScrollSpeedMulti();

	//重力更新
	if (m_moveY > FALLSPEEDMAX)
	{
		//上重力にもする
		float gravity = GRAVITY;
		if (m_mode == VisibleMode::ToInvisible_OnDead)gravity = -gravity;

		m_moveY -= gravity;
		if (m_moveY <= FALLSPEEDMAX)m_moveY = FALLSPEEDMAX;
	}

	//死んでいたらデゾルブ（の代わりに透明化）
	if (m_isDead)
	{
		m_countF_fadeStart--;
		if (m_countF_fadeStart <= 0)m_mode = VisibleMode::ToInvisible_OnDead;
	}

	//透明更新
	UpdateAlpha();

	//角度更新
	m_angleDeg += TERRAINBASEROTATY * speedMulti;
	//死んでいたらさらに追加
	if (m_isDead)m_angleDeg += TERRAINBASEROTATY * SPEEDMULTIADD_DEAD;
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	//角度に合わせて位置変更
	m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos,m_pos.y += m_moveY + TERRAINBASEMOVEY * speedMulti,cosf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos };
	//死んでいたらさらに変更
	if (m_isDead)m_pos.y += TERRAINBASEMOVEY * SPEEDMULTIADD_DEAD;

	//アニメーション更新
	UpdateAnim();
	
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = trans;
}

void Ghost::HitCheck()
{
	//死んでいたらスキップ
	if (!m_isDead && m_isVisibleStart)
	{
		//透明化するかのチェック
		//スフィア判定

		//変数作成
		KdCollider::SphereInfo sphere;

		//スフィアの中心座標
		sphere.m_sphere.Center = m_pos + m_sphereHitOfs;

		//スフィアの半径
		sphere.m_sphere.Radius = 0.6f;

		//当たり判定をしたいタイプ
		sphere.m_type = KdCollider::Type::TypeRollingObj;

		//スフィアに当たったオブジェクト情報を格納するリスト
		std::list<KdCollider::CollisionResult> retSphereList;

		//全オブジェクトとの当たり判定
		for (auto& obj : SceneManager::Instance().GetObjList())
		{
			//↓スフィア当たり判定
			obj->Intersects(sphere, &retSphereList);
		}

		if (!retSphereList.empty())
		{
			m_mode = VisibleMode::ToInvisible;
		}
		else
		{
			m_mode = VisibleMode::ToVisible;
		}
	}

	//本体
	EnemyBase::HitCheck();
}

void Ghost::PostUpdate()
{
	//本体呼ぶ
	EnemyBase::PostUpdate();

	//可視化フラグ
	if (m_pos.y < VISIBLEY && !m_isVisibleStart)
	{
		m_mode = VisibleMode::ToVisible;
		m_isVisibleStart = true;
	}
}

void Ghost::DrawLit()
{
	Math::Color color = Math::Color(1.0f, 1.0f, 1.0f, m_alpha);
	if (m_alpha < 1.0f)KdShaderManager::Instance().ChangeDepthStencilState(KdDepthStencilState::ZWriteDisable);
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygons->find(m_nowAnim)->second.m_polygon, m_mWorld, color);
	if (m_alpha < 1.0f)KdShaderManager::Instance().ChangeDepthStencilState(KdDepthStencilState::ZEnable);
}

void Ghost::GenerateDepthMapFromLight()
{
	if (m_isVisible)
	{
		Math::Color color = Math::Color(1.0f, 1.0f, 1.0f, m_alpha);
		KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygons->find(m_nowAnim)->second.m_polygon, m_mWorld, color);
	}
}

void Ghost::Init()
{
	m_sphereHitOfs = SPHEREHITOFS;
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Ghost", SPHEREHITOFS, 0.6f, KdCollider::TypeDamage);
	m_shadowY = SPAWNPOS.y;
}

void Ghost::Release()
{}

void Ghost::UpdateAlpha()
{
	switch (m_mode)
	{
	case VisibleMode::ToVisible:
		if (m_alpha <= 1.0f)
		{
			m_alpha += ALPHACHANGE;
			if (m_alpha >= 1.0f)
			{
				m_alpha = 1.0f;
				m_mode = VisibleMode::Idle;
			}
		}
		break;
	case VisibleMode::ToInvisible:
		if (m_alpha >= INVISIBLEALPHA)
		{
			m_alpha -= ALPHACHANGE;
			if (m_alpha <= INVISIBLEALPHA)
			{
				m_alpha = INVISIBLEALPHA;
				m_mode = VisibleMode::Idle;
			}
		}
		break;
	case VisibleMode::ToInvisible_OnDead:
		if (m_alpha >= 0.0f)
		{
			m_alpha -= ALPHACHANGE_DEAD;
			if (m_alpha <= 0.0f)
			{
				m_alpha = 0.0f;
				m_isExpired = true;
			}
		}
		break;
	}

	if (m_alpha <= INVISIBLEALPHA)m_isVisible = false;
	else m_isVisible = true;

	//当たり判定更新
	m_pCollider->SetEnableAll(m_isVisible);
}

void Ghost::OnHit()
{
	//透明状態なら無敵
	if (m_isVisible)
	{
		//透明化準備
		m_countF_fadeStart = FADESTART;

		EnemyBase::OnHit();
	}
}
