#include "Player.h"
#include "../../../Scene/SceneManager.h"

Player::Player()
{
	Init();
	m_pos = { 0,0,LINEPOSSTART };
}

void Player::Update()
{
	//重力更新
	m_moveY -= GRAVITY;
	
	//奥行き移動
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_pos.z += LINEMOVE;
		if (m_pos.z > LINEPOSMAX)m_pos.z = LINEPOSMAX;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pos.z -= LINEMOVE;
		if (m_pos.z < LINEPOSMIN)m_pos.z = LINEPOSMIN;
	}

	//ジャンプ
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!m_isJump)
		{
			m_isJump = true;
			m_moveY = JUMPPOWER;
		}
	}

	//空中ジャンプ規制
	if (m_moveY < AIRJUMPLIMIT && !m_isJump)m_isJump = true;

	//Y座標更新
	m_pos.y += m_moveY;

	//アニメーション更新
	m_animCnt += ANIMADD;
	if (m_animCnt >= ANIMMAX)m_animCnt -= ANIMMAX;
	m_polygon->SetUVRect((int)m_animCnt);

	//スクロール量更新
	m_stageScrollMulti += STAGESCROLLADD;
	if (m_stageScrollMulti > STAGESCROLLMAX)m_stageScrollMulti = STAGESCROLLMAX;
	SCENEMGR.SetScrollSpeedMulti(m_stageScrollMulti);
}

void Player::PostUpdate()
{
	//1
	//当たり判定（レイ判定）

	//当たり判定を実装するときは当たる側と当たられる側が存在する
	//当たる側の処理

	//↓ここからレイ判定

	//レイ判定用の変数を作成
	const float enableStepHigh = 1.0f;
	KdCollider::RayInfo ray;
	//レイの発射位置を設定
	ray.m_pos = m_pos + Math::Vector3(0, enableStepHigh, 0);
	//レイの発射方向を設定
	ray.m_dir = { 0,-1.0f,0 };
	//レイの長さを設定
	ray.m_range = enableStepHigh - m_moveY;
	//当たり判定をしたいタイプを設定
	ray.m_type = KdCollider::TypeGround;

	//デバッグ用
	m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range);

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
		m_isJump = false;
		m_moveY = 0.0f;
	}

	//2
	//当たり判定（スフィア判定）

	//変数作成
	KdCollider::SphereInfo sphere;

	//スフィアの中心座標
	sphere.m_sphere.Center = m_pos + Math::Vector3(0, 2.0f, 0);

	//スフィアの半径
	sphere.m_sphere.Radius = 1.0f;

	//当たり判定をしたいタイプ
	sphere.m_type = KdCollider::Type::TypeDamage;

	//デバッグ用
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius);

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

	if (m_pos.y < VOIDPOSY)
	{
		FallVoid();
	}
}

void Player::DrawLit()
{
	//マトリックス
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);

	//Worldセット
	m_mWorld = trans;

	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld);
}

void Player::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld);
}

void Player::OnHit()
{
	//ジャンプ不可に
	m_isJump = true;
	//上方向に飛ばす
	m_moveY = HITJUMP;
	//被弾フラグ
	m_isHit = true;

	//スクロールを戻す
	m_stageScrollMulti = STAGESCROLL_ONHIT;
}

void Player::FallVoid()
{
	//ジャンプ不可に
	m_isJump = true;
	//上方向に飛ばす
	m_moveY = HITJUMP_VOID;
	//被弾フラグ
	m_isHit = true;

	//スクロールを戻す
	m_stageScrollMulti = STAGESCROLL_VOID;
}

void Player::Init()
{
	//ポリゴン初期化
	m_polygon = std::make_shared<KdSquarePolygon>();
	m_polygon->SetMaterial("Asset/Textures/Chara/Player/PlayerRun.png");
	m_polygon->SetSplit(12, 1);
	m_polygon->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	m_polygon->SetScale(4.0f);

	//デバッグ
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Player::Release()
{}
