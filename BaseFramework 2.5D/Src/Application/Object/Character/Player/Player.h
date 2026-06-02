#pragma once
#include "../../../Const/StageConst.h"

class Player :public KdGameObject
{
public:

	Player();
	~Player()override { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;
	void OnHit();
	void FallVoid();

	//ゲッター
	float GetStageScroll()const {};

private:

	void Init()override;
	void Release();

	std::shared_ptr<KdSquarePolygon> m_polygon;

	const float LINEPOSMAX = -8.0f;
	const float LINEPOSSTART = -22.0f;
	const float LINEPOSMIN = -36.0f;
	const float LINEMOVE = 0.3f;

	const float GRAVITY = 0.05f;
	const float JUMPPOWER = 0.75f;
	const float HITJUMP = 0.8f;
	const float HITJUMP_VOID = 1.25f;

	Math::Vector3 m_pos;
	float m_moveY = 0.0f;
	float m_animCnt = 0.0f;
	const float ANIMMAX = 11;
	const float ANIMADD = 0.5f;

	//空中ジャンプ規制（少し猶予を設ける）
	const float AIRJUMPLIMIT = -0.2f;

	bool m_isJump = false;

	//被弾フラグ
	bool m_isHit = false;

	//奈落位置
	const float VOIDPOSY = -10.0f;

	//ステージスクロール量
	const float STAGESCROLL_ONHIT = -2.5f;
	const float STAGESCROLL_VOID = -2.0f;
	const float STAGESCROLLADD = 0.05f;
	const float STAGESCROLLMAX = 1.0f;

	float m_stageScrollMulti = STAGESCROLLMAX;
};