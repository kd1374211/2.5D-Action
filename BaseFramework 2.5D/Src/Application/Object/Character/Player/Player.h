#pragma once

class Player :public KdGameObject
{
public:

	Player(Math::Vector3 a_startPos);
	~Player()override { Release(); }

	void Update()override;
	void DrawLit()override;

private:

	void Init()override;
	void Release();

	std::shared_ptr<KdSquarePolygon> m_polygon;

	const float LINEPOSMIN = 16.0f;
	const float LINEPOSSTART = 48.0f;
	const float LINEPOSMAX = 80.0f;
	const float LINEMOVE = 1.0f;

	float m_linePos;
	Math::Vector3 m_pos;
	float m_angleDeg;
	int m_animCnt;
	static const int ANIMMAX = 32;

};