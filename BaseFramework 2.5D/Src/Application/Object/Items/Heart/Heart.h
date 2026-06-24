#pragma once

class Heart :public KdGameObject
{
public:

	Heart(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos);
	~Heart()override { Release(); }

	void Update()override;
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

};