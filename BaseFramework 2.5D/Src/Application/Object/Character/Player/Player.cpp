#include "Player.h"
#include "../../../Scene/SceneManager.h"

Player::Player()
{
	Init();
	m_pos = { 0,0.2f,LINEPOSSTART };

	for (int i = 0; i < STARTHEALTH; i++)
	{
		m_healthTexData.push_back(HealthTexData(i, false, 0.0f, false));
	}
}

void Player::PreUpdate()
{
	CheckHeartAnimExpired();
}

void Player::Update()
{
	//重力更新
	if (m_moveY > FALLSPEEDMAX)
	{
		m_moveY -= GRAVITY;
		if (m_moveY <= FALLSPEEDMAX)m_moveY = FALLSPEEDMAX;
	}
	
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
			ChangeAnim(PlayerAnimType::Jump);
		}
	}

	//空中ジャンプ規制
	if (m_moveY < AIRJUMPLIMIT && !m_isJump)m_isJump = true;

	//攻撃
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		ChangeAnim(PlayerAnimType::Attack);
	}

	//Y座標更新
	m_pos.y += m_moveY;

	//アニメーション更新
	UpdateAnim();
	
	//ハート画像
	UpdateHeartAnim();

	//無敵F更新
	if (m_isInvinsible)
	{
		m_immuneF--;
		if (m_immuneF <= 0)
		{
			m_isInvinsible = false;
			ChangeAnim(PlayerAnimType::Run);
		}
	}

	//スクロール量更新
	m_stageScrollMulti += STAGESCROLLADD;
	if (m_stageScrollMulti > STAGESCROLLMAX)m_stageScrollMulti = STAGESCROLLMAX;
	SCENEMGR.SetScrollSpeedMulti(m_stageScrollMulti);
}

void Player::HitCheck()
{
	//1
	//当たり判定（レイ判定）

	//当たり判定を実装するときは当たる側と当たられる側が存在する
	//当たる側の処理

	//上飛び中は地面判定スキップ
	SCENEMGR.SetScrollBack(0.0f);
	if (m_moveY <= 0.0f)
	{
		//↓ここからレイ判定

		//レイ判定用の変数を作成
		const float enableStepHigh = 0.2f;
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
			if (m_nowAnim == PlayerAnimType::Jump)ChangeAnim(PlayerAnimType::Run);
		}

		//2
		//当たり判定（スフィア判定）

		//変数作成
		KdCollider::SphereInfo sphere;

		//スフィアの中心座標
		sphere.m_sphere.Center = m_pos + Math::Vector3(0, 0.4f, 0);

		//スフィアの半径
		sphere.m_sphere.Radius = m_pos.z * SPHEREGROUNDHITSIZEMULTI;

		//当たり判定をしたいタイプ
		sphere.m_type = KdCollider::TypeGround;

		//デバッグ用
		m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius);

		//スフィアに当たったオブジェクト情報を格納するリスト
		std::list<KdCollider::CollisionResult> retSphereList;

		//全オブジェクトとの当たり判定
		for (auto& obj : SCENEMGR.GetObjList())
		{
			//↓スフィア当たり判定
			obj->Intersects(sphere, &retSphereList);
		}

		//スフィアに当たったリストから一番近いオブジェクトを探す

		//↓レイの時にあるので使いまわし
		maxOverLap = 0.0f;		//スフィアのときはめり込んだ長さ
		isHit = false;

		//当たった方向
		float deg;
		Math::Vector3 hitDir;
		
		for (auto& ret : retSphereList)
		{
			//スフィアにめり込んだ長さが一番長いものを探す
			if (maxOverLap < ret.m_overlapDistance)
			{
				hitPos = ret.m_hitPos;

				//衝突方向を見る(Y消し)
				hitDir = ret.m_hitDir;
				
				//衝突位置の角度を求める
				deg = DirectX::XMConvertToDegrees(atan2f(hitPos.z, hitPos.x));

				//更新
				maxOverLap = ret.m_overlapDistance;
				isHit = true;
			}
		}

		if (isHit)
		{
			//一定以上プレイヤーのYが高いなら登る
			if (m_pos.y > -0.15f)
			{
				m_pos.y = hitPos.y;
				m_moveY = 0.0f;
				m_isJump = false;
				if (m_nowAnim == PlayerAnimType::Jump)ChangeAnim(PlayerAnimType::Run);
			}
			else
			{
				//押した先の位置を見る
				Math::Vector3 pushPos = m_pos + hitDir * maxOverLap;
				
				//移動先の角度を求める
				deg = DirectX::XMConvertToDegrees(atan2f(pushPos.z, pushPos.x));

				//スクロールを戻す
				SCENEMGR.SetScrollBack(-90.0f - deg);
			}
		}
	}

	//3
	//被弾判定（スフィア判定）

	//変数作成
	KdCollider::SphereInfo sphere;

	//スフィアの中心座標
	sphere.m_sphere.Center = m_pos + Math::Vector3(0, 0.4f, 0);

	//スフィアの半径
	sphere.m_sphere.Radius = m_pos.z * SPHEREHITSIZEMULTI;

	//当たり判定をしたいタイプ
	sphere.m_type = KdCollider::Type::TypeDamage;

	//デバッグ用
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kRedColor);

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

	KdDebugGUI::Instance().AddLog("PlayerPosY : %.2f\n", m_pos.y);
	KdDebugGUI::Instance().AddLog("PlayerLinePos : %.2f\n", m_pos.z);

	//マトリックス
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = trans;
}

void Player::DrawLit()
{
	PolygonData* data = &m_polygons->find(m_nowAnim)->second;
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*data->m_polygon, m_mWorld);
}

void Player::GenerateDepthMapFromLight()
{
	PolygonData* data = &m_polygons->find(m_nowAnim)->second;
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*data->m_polygon, m_mWorld);
}

void Player::DrawSprite()
{
	int heartNum = m_healthTexData.size();
	float left = -630.0f;
	for (auto &itr : m_healthTexData)
	{
		if (itr.m_isExpired)continue;

		Math::Rectangle rec = Math::Rectangle((long)((int)itr.m_animCnt * HEARTTEXBASESIZE.x), 0, (long)HEARTTEXBASESIZE.x, (long)HEARTTEXBASESIZE.y);
		Math::Vector2 pos = Math::Vector2(left + HEARTTEXDRAWSIZE.x * ((float)itr.m_number + 0.5f), 310.0f);
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_heartTex, pos.x, pos.y, HEARTTEXDRAWSIZE.x, HEARTTEXDRAWSIZE.y, &rec);
	}
}

void Player::OnHit()
{
	//無敵ならスキップ
	if (m_isInvinsible)return;
	//ジャンプ不可に
	m_isJump = true;
	//上方向に飛ばす
	m_moveY = HITJUMP;
	
	//スクロールを戻す
	m_stageScrollMulti = STAGESCROLL_ONHIT;

	OnDamage();
}

void Player::FallVoid()
{
	//ジャンプ不可に
	m_isJump = true;
	//上方向に飛ばす
	m_moveY = HITJUMP_VOID;
	
	//スクロールを戻す
	m_stageScrollMulti = STAGESCROLL_VOID;

	//無敵ならノーダメージ
	if (m_isInvinsible)return;

	OnDamage();
}

void Player::Respawn()
{
	//初期化
	m_pos = { 0,0.2f,LINEPOSSTART };
	m_moveY = 0.0f;
	m_isJump = false;

	ChangeAnim(PlayerAnimType::Run);
	m_isDead = false;
	m_isGameEnd = false;

	m_health = STARTHEALTH;
	for (int i = 0; i < STARTHEALTH; i++)
	{
		m_healthTexData.push_back(HealthTexData(i, false, 0.0f, false));
	}

	m_isInvinsible = true;
	m_immuneF = HITIMMUNEF;

	m_stageScrollMulti = STAGESCROLLMAX;
}

void Player::Init()
{
	//ハート画像
	m_heartTex = std::make_shared<KdTexture>();
	m_heartTex->Load("Asset/Textures/Heart.png");

	//デバッグ
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Player::Release()
{}

void Player::ChangeAnim(PlayerAnimType a_anim)
{
	m_nowAnim = a_anim;
	m_animCnt = 0.0f;
}

void Player::UpdateAnim()
{
	bool isAnimLooped = false;

	PolygonData* data = &m_polygons->find(m_nowAnim)->second;

	m_animCnt += data->m_animAdd;
	if (m_animCnt >= data->m_animMax)
	{
		m_animCnt -= data->m_animMax;
		isAnimLooped = true;
	}

	//画像が1周したらなにかあるやつ
	if (isAnimLooped)
	{
		switch (m_nowAnim)
		{
		case PlayerAnimType::Attack:
			ChangeAnim(PlayerAnimType::Run);
			break;
		case PlayerAnimType::Dead:
			break;
		default:
			break;
		}

		//再検索
		data = &m_polygons->find(m_nowAnim)->second;
	}

	data->m_polygon->SetUVRect((int)m_animCnt);
}

void Player::CheckHeartAnimExpired()
{
	//消去チェック
	while (!m_healthTexData.empty() && m_healthTexData.back().m_isExpired)
	{
		m_healthTexData.pop_back();
	}
}

void Player::UpdateHeartAnim()
{
	for (auto& itr : m_healthTexData)
	{
		if (itr.m_isAnimStart)
		{
			itr.m_animCnt += HEALTHANIMSPEED;

			if (itr.m_animCnt >= HEALTHANIMEND)itr.m_isExpired = true;
		}
	}
}

void Player::OnDamage()
{
	if (m_isInvinsible)return;
	m_health--;
	if (m_health <= 0)
	{
		ChangeAnim(PlayerAnimType::Dead);
		//死亡
		m_isDead = true;
	}
	else
	{
		ChangeAnim(PlayerAnimType::Hit);
	}

	//被弾後無敵
	m_isInvinsible = true;
	m_immuneF = HITIMMUNEF;

	//体力画像
	if (!m_healthTexData.empty())m_healthTexData.back().m_isAnimStart = true;
}
