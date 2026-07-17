#pragma once
#include "../CharaBase.h"

class EnemyBase :public CharaBase
{
public:

	EnemyBase() {};
	~EnemyBase()override { Release(); }

	virtual void HitCheck()override;
	virtual void PostUpdate()override;
	void PreDraw()override;
	void GenerateDepthMapFromLight()override;

	//セッター
	void SetPolygonData(std::map<EnemyAnimType, PolygonData>* a_data) { m_polygons = a_data; }
	void SetEnemyID(int a_value) { m_enemyID = a_value; }

	//ゲッター
	float GetAngleDeg()const { return m_angleDeg; }
	float GetLinePos()const { return m_linePos; }
	int GetEnemyID()const { return m_enemyID; }
	Math::Vector3 GetHitOfsPos()const { return m_sphereHitOfs; }

protected:

	void ChangeAnim(EnemyAnimType a_anim);
	virtual void UpdateAnim();
	virtual void OnHit();
	void Release();

	//撃破時増加ハートチャージ量
	const float HEARTCHARGEADD = 15.0f;

	//死亡時
	const float SPEEDMULTIADD_DEAD = -1.75f;
	const float JUMPPOWER_DEAD = 0.14f;

	float m_angleDeg = 0.0f;
	float m_linePos = 0.0f;
	EnemyAnimType m_nowAnim = EnemyAnimType::Idle;
	std::map<EnemyAnimType, PolygonData>* m_polygons = nullptr;

	//スフィア判定Pos差
	Math::Vector3 m_sphereHitOfs = Math::Vector3::Zero;

	//敵ID
	int m_enemyID = 0;

	float m_shadowY = 0.0f;
};