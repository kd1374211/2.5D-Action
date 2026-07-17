#include "FlyEye.h"
#include "../../../../Scene/SceneManager.h"

FlyEye::FlyEye(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos)
{
	Init();
	m_pos = a_baseStartPos + SPAWNPOS;
	m_angleDeg = a_startDeg;
	m_linePos = a_linePos;
}

void FlyEye::Update()
{
	float speedMulti = SCENEMGR.GetScrollSpeedMulti();

	//死んでいたら重力更新・デゾルブ
	if (m_isDead)
	{
		if (m_moveY > FALLSPEEDMAX)
		{
			m_moveY -= GRAVITY;
			if (m_moveY <= FALLSPEEDMAX)m_moveY = FALLSPEEDMAX;
		}

		m_countF_dissolveStart--;
		if (m_countF_dissolveStart <= 0)m_isDissolve = true;
	}

	//仮置き
	if (m_isEvadeMode)
	{
		if (!m_isRiseEnd)
		{
			//上昇
			if (m_countF_evade < RISEF)
			{
				m_pos.y += FLYPOWER;
			}

			//上昇終了チェック
			if (m_countF_evade >= RISEF + WAITF)m_isRiseEnd = true;
		}

		//下降
		if (m_isFallStart)
		{
			if (m_countF_evade < FALLF)
			{
				m_pos.y -= FLYPOWER;
			}
			//終了
			else
			{
				m_isEvadeMode = false;
				m_isRiseEnd = false;
				m_isFallStart = false;
			}
		}

		//フレーム更新
		if (m_countF_evade < RISEF + WAITF)m_countF_evade++;
	}

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

void FlyEye::HitCheck()
{
	//死んでいたらスキップ
	if (!m_isDead && !m_isEvadeMode)
	{
		//転がってくる岩を避けるかどうか
		//仮想Deg設定
		float hitAngle = m_angleDeg + TERRAINBASEROTATY * DEGDIFF_ROLLOBJHIT;

		//それに合わせて仮想位置設定
		Math::Vector3 baseHitPos = { sinf(DirectX::XMConvertToRadians(hitAngle)) * m_linePos,m_pos.y + m_moveY + TERRAINBASEMOVEY * DEGDIFF_ROLLOBJHIT,cosf(DirectX::XMConvertToRadians(hitAngle)) * m_linePos };

		//スフィア判定

		//変数作成
		KdCollider::SphereInfo sphere;

		//スフィアの中心座標
		sphere.m_sphere.Center = baseHitPos + m_sphereHitOfs;

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
			m_isEvadeMode = true;
			m_countF_evade = 0;
		}
	}

	//下降開始チェック
	//死んでいたらスキップ-2
	if (!m_isDead && m_isRiseEnd && !m_isFallStart)
	{
		//転がってくる岩を避けるかどうか
		//仮想Pos設定
		Math::Vector3 baseHitPos = m_pos + Math::Vector3(0, POSDIFFY_FALLCHECK, 0);

		//スフィア判定

		//変数作成
		KdCollider::SphereInfo sphere;

		//スフィアの中心座標
		sphere.m_sphere.Center = baseHitPos + m_sphereHitOfs;

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

		if (retSphereList.empty())
		{
			m_isFallStart = true;
			m_countF_evade = 0;
		}
	}

	//基底呼び
	EnemyBase::HitCheck();
}

void FlyEye::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dissolvePow);
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygons->find(m_nowAnim)->second.m_polygon, m_mWorld);
	KdShaderManager::Instance().m_StandardShader.SetDissolve(0);
}

void FlyEye::Init()
{
	m_sphereHitOfs = SPHEREHITOFS;
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Fly Eye", SPHEREHITOFS, 0.6f, KdCollider::TypeDamage);

	m_shadowY = SPAWNPOS.y;
}

void FlyEye::Release()
{

}

void FlyEye::UpdateAnim()
{
	//本体
	EnemyBase::UpdateAnim();

	//デゾルブ
	if (m_isDissolve)
	{
		m_dissolvePow += DISSOLVEADD;
		if (m_dissolvePow >= 1.0f)m_isExpired = true;
	}
}

void FlyEye::OnHit()
{
	//避けモード終了
	m_isEvadeMode = false;

	//デゾルブ準備
	m_countF_dissolveStart = DISSOLVESTART;

	//本体
	EnemyBase::OnHit();
}
