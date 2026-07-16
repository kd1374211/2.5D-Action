#include "Heart.h"
#include "../../../Scene/SceneManager.h"

Heart::Heart(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos, float a_targetLinePos, int a_targetStairDiff)
{
	Init();

	m_pos = a_baseStartPos + SPAWNPOS;
	m_angleDeg = a_startDeg;
	m_linePos = a_linePos;
	m_countF = a_targetStairDiff * COUNTF_SINGLESTAIR;
	float lineMoveDist = a_targetLinePos - a_linePos;
	m_moveSpeedY = GRAVITY * (float)m_countF / 2.0f;
	m_lineMoveSpeed = lineMoveDist / (float)m_countF;
}

void Heart::Update()
{
	//カウント更新
	if (m_countF > 0)m_countF--;

	//スクロール速度取得
	float speedMulti = SCENEMGR.GetScrollSpeedMulti();

	//Degずらし
	if (m_countF > 0)speedMulti -= DEGMOVE;

	//LinePosずらし
	if (m_countF > 0)m_linePos += m_lineMoveSpeed;

	//重力更新
	m_moveSpeedY -= GRAVITY;
	if (m_moveSpeedY < FALLSPEEDLIMIT)m_moveSpeedY = FALLSPEEDLIMIT;

	//角度更新
	m_angleDeg += TERRAINBASEROTATY * speedMulti;
	if (m_angleDeg > 360.0f)m_angleDeg -= 360.0f;
	else if (m_angleDeg < 0.0f)m_angleDeg += 360.0f;

	//角度に合わせて位置変更
	m_pos = { sinf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos,m_pos.y += TERRAINBASEMOVEY * speedMulti + m_moveSpeedY,cosf(DirectX::XMConvertToRadians(m_angleDeg)) * m_linePos };

	//アニメーション更新
	UpdateAnim();

	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = trans;
}

void Heart::HitCheck()
{
	//レイ判定用の変数を作成
	const float enableStepHigh = 0.4f;
	KdCollider::RayInfo ray;
	//レイの発射位置を設定
	ray.m_pos = m_pos + Math::Vector3(0, enableStepHigh, 0);
	//レイの発射方向を設定
	ray.m_dir = { 0,-1.0f,0 };
	//レイの長さを設定
	ray.m_range = enableStepHigh - m_moveSpeedY;
	//当たり判定をしたいタイプを設定
	ray.m_type = KdCollider::TypeGround;

	//レイに当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retRayList;

	//全オブジェクトとの当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		//↓レイ当たり判定
		obj->Intersects(ray, &retRayList);
	}

	//レイに当たったリストから一番近いオブジェクトを探す
	float maxOverLap = 0.0f;
	Math::Vector3 hitPos;
	bool isHit = false;
	for (auto& ret : retRayList)
	{
		//レイを遮断してオーバーした長さが一番長いものを探す
		if (maxOverLap < ret.m_overlapDistance)
		{
			//更新
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			isHit = true;
		}
	}

	//座標更新
	if (isHit)
	{
		m_pos = hitPos;
		
		//着地
		if (!m_isLanding)
		{
			m_isLanding = true;
			m_pCollider->SetEnableAll(true);
		}

		//落下速度リセット
		m_moveSpeedY = 0.0f;
	}
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
	m_polygon->SetMaterial("Asset/Textures/Heart/Heart.png");
	m_polygon->SetSplit(12, 1);
	m_polygon->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);

	//当たり判定
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Heart", SPHEREHITOFS, 0.5f, KdCollider::TypeHeal);
	m_pCollider->SetEnableAll(false);
}

void Heart::Release()
{}

void Heart::UpdateAnim()
{
	m_animCnt += ANIMADD;
	if (m_animCnt >= ANIMMAX)m_animCnt -= ANIMMAX;
}
