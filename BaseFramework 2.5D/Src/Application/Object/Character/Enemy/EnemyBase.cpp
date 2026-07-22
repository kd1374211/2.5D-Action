#include "EnemyBase.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Score/ScoreManager.h"
#include "../../../StageSpawner/StageSpawner.h"
#include "../../../Sound/SoundManager.h"
#include "../../../Camera/CameraManager.h"
#include "../../../HeartCharge/HeartCharge.h"

void EnemyBase::HitCheck()
{
	//1
	//当たり判定（レイ判定）

	//当たり判定を実装するときは当たる側と当たられる側が存在する
	//当たる側の処理

	//↓ここからレイ判定

	//レイ判定用の変数を作成
	const float enableStepHigh = 0.4f;
	KdCollider::RayInfo ray;
	//レイの発射位置を設定
	ray.m_pos = m_pos + Math::Vector3(0, enableStepHigh, 0);
	//レイの発射方向を設定
	ray.m_dir = { 0,-1.0f,0 };
	//レイの長さを設定
	ray.m_range = enableStepHigh - m_moveY;
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
		m_moveY = 0.0f;
	}

	//死んでいたら判定スキップ
	if (m_isDead)return;

	//3
	//被弾判定（スフィア判定）

	//変数作成
	KdCollider::SphereInfo sphere;

	//スフィアの中心座標
	sphere.m_sphere.Center = m_pos + m_sphereHitOfs;

	//スフィアの半径
	sphere.m_sphere.Radius = m_linePos * 0.125f;
	if (sphere.m_sphere.Radius <= 0.6f)sphere.m_sphere.Radius = 0.6f;

	//当たり判定をしたいタイプ
	sphere.m_type = KdCollider::Type::TypeDamage_Enemy;

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
		OnHit();
	}
}

void EnemyBase::PostUpdate()
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
	m_isShadow = m_pos.y < SHADOWDRAWSTART + m_shadowY ? true : false;

	Math::Matrix rotatY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(210.0f));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = rotatY * trans;
}

void EnemyBase::PreDraw()
{
	PolygonData* data = &m_polygons->find(m_nowAnim)->second;
	data->m_polygon->SetUVRect((int)m_animCnt);
}

void EnemyBase::GenerateDepthMapFromLight()
{
	if (!m_isShadow)return;
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygons->find(m_nowAnim)->second.m_polygon, m_mWorld);
}

void EnemyBase::ChangeAnim(EnemyAnimType a_anim)
{
	m_nowAnim = a_anim;
	m_animCnt = 0.0f;
}

void EnemyBase::UpdateAnim()
{
	PolygonData* data = &m_polygons->find(m_nowAnim)->second;

	m_animCnt += data->m_animAdd;
	if (m_animCnt >= data->m_animMax)
	{
		//画像が1周したらなにかあるやつ
		switch (m_nowAnim)
		{
		case EnemyAnimType::Hit:
			m_animCnt -= data->m_animAdd;
			break;
		default:
			m_animCnt -= data->m_animMax;
			break;
		}
	}
}

void EnemyBase::OnHit()
{
	//撃破数増加
	SCOREMGR.AddKillCount();

	//フラグ関係
	m_isDead = true;
	m_moveY = JUMPPOWER_DEAD;

	//判定消滅
	m_pCollider->SetEnableAll(false);

	//アニメーション
	ChangeAnim(EnemyAnimType::Hit);

	//撃破音
	SOUNDMGR.Play(SoundName::SE_AttackHit);

	//ハートチャージ更新
	HEARTCHARGE.OnEnemyDead();

	//ハートとエフェクト
	STAGESPAWNER.OnEnemyDead(m_enemyID);

	//スクリーンシェイク
	CAMERAMGR.SetCameraShakeFlg(true);
}

void EnemyBase::Release()
{
	STAGESPAWNER.DeleteEnemyMap(m_enemyID);
}
