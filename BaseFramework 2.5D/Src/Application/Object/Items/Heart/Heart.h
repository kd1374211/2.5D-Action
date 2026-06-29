#pragma once

class Heart :public KdGameObject
{
public:

	Heart(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos, float a_targetLinePos, int a_targetStairDiff);
	~Heart()override { Release(); }

	void Update()override;
	void HitCheck()override;
	void PostUpdate()override;
	void PreDraw()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

private:

	void Init()override;
	void Release();
	void UpdateAnim();
	
	//召喚時にずらすY
	const Math::Vector3 SPAWNPOS = Math::Vector3(0, 0.5f, 0);

	//球判定Pos
	const Math::Vector3 SPHEREHITOFS = Math::Vector3(0, 0.5f, 0);

	float m_angleDeg;
	float m_linePos;

	//影対策
	bool m_isShadow = false;

	//アニメーション
	const float ANIMMAX = 12.0f;
	const float ANIMADD = 0.25f;
	float m_animCnt = 0.0f;
	std::shared_ptr<KdSquarePolygon> m_polygon = nullptr;

	//吹っ飛び
	const int COUNTF_SINGLESTAIR = 6;
	const float DEGMOVE = 1.5f;
	const float GRAVITY = 0.005f;
	const float FALLSPEEDLIMIT = -0.2f;

	int m_countF = 0;
	float m_isLanding = false;
	float m_moveSpeedY = 0.0f;
	float m_lineMoveSpeed = 0.0f;
};