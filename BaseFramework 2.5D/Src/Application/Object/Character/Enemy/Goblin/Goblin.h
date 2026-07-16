#pragma once
#include "../EnemyBase.h"

class Goblin :public EnemyBase
{
public:

	Goblin(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos);
	~Goblin()override { Release(); }

	void Update()override;
	void HitCheck()override;
	void PostUpdate()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

private:

	enum VisibleMode
	{
		ToVisible,
		Idle,
		ToInvisible
	};

	//出現位置
	const Math::Vector3 SPAWNPOS = Math::Vector3(0, 0.24f, 0);

	//球判定Pos
	const Math::Vector3 SPHEREHITOFS = Math::Vector3(0, 0.6f, 0);

	//可視化位置
	const float VISIBLEY = 1.2f;
	const float ALPHACHANGE = 0.05f;

	//透明状態の透明度
	const float INVISIBLEALPHA = 0.2f;

	//透明度
	bool m_isVisibleStart = false;

	float m_alpha = INVISIBLEALPHA;
	VisibleMode m_mode = VisibleMode::Idle;
	bool m_isVisible = false;

	void Init()override;
	void Release();

	void UpdateAlpha();
	void OnHit()override;

};