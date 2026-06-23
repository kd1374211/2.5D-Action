#pragma once
#include "../CharaBase.h"

class EnemyBase :public CharaBase
{
public:

	EnemyBase() {}
	~EnemyBase()override {}

	virtual void HitCheck()override;
	void PostUpdate()override;
	void PreDraw()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

	//セッター
	void SetPolygonData(std::map<EnemyAnimType, PolygonData>* a_data) { m_polygons = a_data; }

protected:

	void ChangeAnim(EnemyAnimType a_anim);
	void UpdateAnim();
	void OnHit();

	//デゾルブ消滅
	const float DISSOLVEADD = 0.05f;

	bool m_isDissolve = false;
	float m_dissolvePow = 0.0f;

	float m_angleDeg = 0.0f;
	float m_linePos = 0.0f;
	EnemyAnimType m_nowAnim = EnemyAnimType::Idle;
	std::map<EnemyAnimType, PolygonData>* m_polygons = nullptr;

	//スフィア判定Pos差
	Math::Vector3 m_sphereHitOfs = Math::Vector3::Zero;

};