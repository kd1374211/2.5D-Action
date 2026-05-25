#pragma once

class Stair :public KdGameObject
{
public:

	Stair(Math::Vector3 a_startPos, float a_startDeg);
	~Stair()override { Release(); }

	void Update();
	void DrawUnLit();
	void GenerateDepthMapFromLight()override;

private:

	void Init();
	void Release();

	std::shared_ptr<KdModelData> m_model;

	Math::Vector3 m_pos;
	float m_angleDeg;
};