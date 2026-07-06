#pragma once

class Player;

class Slash :public KdGameObject
{
public:

	Slash(Math::Vector3 a_startPos, float a_angle);
	~Slash()override { Release(); }

	void Update()override;
	void DrawEffect()override;

private:

	std::shared_ptr<KdSquarePolygon> m_polygon = nullptr;
	
	const float ANIMMAX = 9.0f;
	const float ANIMADD = 0.75f;
	const float HITEND = 4.0f;

	float m_angleDeg = 0.0f;
	float m_animCnt = 0.0f;

	void Init()override;
	void Release();

};