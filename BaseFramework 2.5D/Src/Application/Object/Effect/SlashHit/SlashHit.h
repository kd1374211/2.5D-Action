#pragma once

class EnemyBase;

class SlashHit :public KdGameObject
{
public:

	SlashHit(std::shared_ptr<EnemyBase> a_target);
	~SlashHit()override { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawEffect()override;

private:

	std::shared_ptr<KdSquarePolygon> m_polygon = nullptr;

	std::weak_ptr<EnemyBase> m_wpTarget;

	const float ANIMMAX = 8.0f;
	const float ANIMADD = 0.5f;
	
	float m_angleDeg = 0.0f;
	float m_animCnt = 0.0f;

	void Init()override;
	void Release();

};