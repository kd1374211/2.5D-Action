#pragma once
#include "../EnemyBase.h"

class FlyEye :public EnemyBase
{
public:

	FlyEye(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos);
	~FlyEye()override { Release(); }

	void Update()override;
	void HitCheck()override;
	void DrawLit()override;
	
private:

	//出現位置
	const Math::Vector3 SPAWNPOS = Math::Vector3(0, 2.0f, 0);

	//球判定Pos
	const Math::Vector3 SPHEREHITOFS = Math::Vector3::Zero;

	//岩避け判定の角度差
	const float DEGDIFF_ROLLOBJHIT = -10.0f;

	//降りチェック
	const float POSDIFFY_FALLCHECK = -1.5f;

	//岩避けモード
	bool m_isEvadeMode = false;
	bool m_isRiseEnd = false;
	bool m_isFallStart = false;

	static const int RISEF = 30;
	static const int WAITF = 50;
	static const int FALLF = 30;
	const float FLYPOWER = 0.05f;

	int m_countF_evade = 0;

	//デゾルブ消滅
	const float DISSOLVEADD = 0.1f;
	static const int DISSOLVESTART = 25;
	
	int m_countF_dissolveStart = 0;
	bool m_isDissolve = false;
	float m_dissolvePow = 0.0f;

	void Init()override;
	void Release();
	void UpdateAnim()override;
	void OnHit()override;

};