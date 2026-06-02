#pragma once

class Boulder :public KdGameObject
{
public:

	Boulder(Math::Vector3 a_startPos, float a_startDeg, float a_linePos);
	~Boulder()override { Release(); }

	void Update()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

private:

	//地形とのスピード差
	const float STAIRDOWNMOVE = -0.05f;
	const float STAIRDOWNROTAT = 0.6f;

	void Init()override;
	void Release();

	std::shared_ptr<KdModelData> m_model;
	Math::Vector3 m_pos;
	float m_angleDeg;
	float m_linePos;
	float m_angleDegZ = 0.0f;

};