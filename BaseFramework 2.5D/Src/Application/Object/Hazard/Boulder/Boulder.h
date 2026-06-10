#pragma once

class Boulder :public KdGameObject
{
public:

	Boulder(Math::Vector3 a_startPos, float a_startDeg, float a_linePos);
	~Boulder()override { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

private:

	//地形とのスピード差
	const float SPEEDMULTIADD = 0.5f;

	void Init()override;
	void Release();

	std::shared_ptr<KdModelData> m_model;
	float m_angleDeg;
	float m_linePos;
	float m_angleDegZ = 0.0f;

};