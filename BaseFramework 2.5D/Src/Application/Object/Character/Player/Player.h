#pragma once
#include "../../../Const/StageConst.h"
#include "../CharaBase.h"

class Player :public CharaBase
{
public:

	Player();
	~Player()override { Release(); }

	void PreUpdate()override;
	void Update()override;
	void HitCheck()override;
	void PostUpdate()override;
	void PreDraw()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;
	void DrawSprite()override;
	void OnHit();
	void FallVoid();

	void Respawn(Math::Vector3 a_pos);
	void Respawn() { Respawn(BASESPAWNPOS); }

	//セッター
	void SetPolygonData(std::map<PlayerAnimType, PolygonData>* a_data) { m_polygons = a_data; }
	void SetIsHeartTex(bool a_flg) { m_isHeartTex = a_flg; }

	//ゲッター
	bool GetIsGameEnd()const { return m_isGameEnd; }
	float GetLinePos()const { return -m_pos.z; }

private:

	void Init()override;
	void Release();

	//スクロール
	void UpdateScrollMulti();

	//アニメーション
	bool ChangeAnimCheck(PlayerAnimType a_anim);
	void ChangeAnim(PlayerAnimType a_anim);
	void UpdateAnim();

	//体力画像
	void CheckHeartAnimExpired();
	void UpdateHeartAnim();

	//体力減少
	void OnDamage();

	//攻撃停止
	void AttackStun();

	//ポリゴン
	std::map<PlayerAnimType, PolygonData>* m_polygons = nullptr;
	
	//体力画像
	const Math::Vector2 HEARTTEXBASESIZE = Math::Vector2(17.0f, 17.0f);
	const Math::Vector2 HEARTTEXDRAWSIZE = HEARTTEXBASESIZE * 4.0f;
	
	std::shared_ptr<KdTexture> m_heartTex = nullptr;

	//移動範囲
	const float LINEPOSMAX = -2.0f;
	const float LINEPOSSTART = -5.0f;
	const float LINEPOSMIN = -8.0f;
	const float LINEMOVE = 0.075f;

	//基礎位置
	const Math::Vector3 BASESPAWNPOS = { 0,0,LINEPOSSTART };

	//当たり判定（スフィア）
	const float SPHEREGROUNDHITSIZEMULTI = -0.05f;
	const float SPHEREHITSIZEMULTI = -0.04f;

	//落下
	const float GRAVITY = 0.01f;
	const float FALLSPEEDMAX = -0.4f;
	const float JUMPPOWER = 0.15f;
	const float HITJUMP = 0.08f;
	const float HITJUMP_DEAD = 0.16f;
	const float HITJUMP_VOID = 0.25f;

	//基礎
	float m_moveY = 0.0f;
	bool m_isJump = false;
	bool m_isVoidJump = false;
	bool m_isAttack = false;
	int m_attackF = 0;
	int m_attackWaitF = 0;

	//攻撃
	static const int ATTACKWAITF_FAIL = 10;
	static const int ATTACKWAITF = 20;
	static const int ATTACKSPAWNF = 4;

	//体力
	static const int STARTHEALTH = 5;
	const float HEALTHANIMSPEED = 0.5f;
	static const int HEALTHANIMEND = 5;

	bool m_isHeartTex = false;

	//ゲーム終了フラグ
	bool m_isGameEnd = false;

	struct HealthTexData
	{
		int m_number;
		bool m_isAnimStart;
		float m_animCnt;
		bool m_isExpired;
	};

	int m_health = STARTHEALTH;
	std::list<HealthTexData> m_healthTexData;

	//アニメーション
	PlayerAnimType m_nowAnim = PlayerAnimType::Run;

	//空中ジャンプ規制（少し猶予を設ける）
	const float AIRJUMPLIMIT = -0.04f;

	//被弾フラグ
	//static const int HITIMMUNEF = 5;
	static const int HITIMMUNEF = 150;

	bool m_isInvinsible = false;
	int m_immuneF = 0;

	//奈落位置
	const float VOIDPOSY = -2.0f;

	//ステージスクロール量
	const float STAGESCROLL_ONHIT = -1.2f;
	const float STAGESCROLL_ONDEAD = -2.5f;
	const float STAGESCROLL_VOID = -2.0f;
	const float STAGESCROLLADD = 0.05f;
	const float STAGESCROLLMAX = 1.0f;
	const float STAGESCROLLADD_DEAD = 0.025f;
	const float STAGESCROLL_GAMEEND = 0.0f;

	float m_stageScrollMulti = STAGESCROLLMAX;

	//キー
	bool m_isAttackKey = false;

	//着地フラグ
	bool m_isLanding = true;
};