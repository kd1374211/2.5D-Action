#pragma once

class Heal :public KdGameObject
{
public:

	Heal() { Init(); }
	~Heal()override { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawBright()override;
	void DrawEffect()override;

private:

	const Math::Vector3 BASEPOSDIFF = Math::Vector3(0, 0.2f, 0);

	void Init()override;
	void Release();
	void UpdateAnim();

	const float ANIMMAX = 10.0f;
	const float ANIMADD = 0.25f;
	const int ANIMSPLIT_X = 6;
	const int ANIMSPLIT_Y = 2;

	const float POSDIFFMOVE = 0.01f;
	const float ALPHACHANGE = -0.02f;

	float m_posDiffY = 0.0f;
	float m_animCnt = 0.0f;
	float m_alpha = 0.8f;
	std::shared_ptr<KdSquarePolygon> m_polygon = nullptr;

};