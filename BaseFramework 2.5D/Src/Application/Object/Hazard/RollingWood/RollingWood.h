#pragma once

class RollingWood :public KdGameObject
{
public:

	RollingWood(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos, float a_length);
	~RollingWood() { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

private:

	//出現位置
	const Math::Vector3 SPAWNPOS = Math::Vector3(0, 0.8f, 0);

	//地形とのスピード差
	const float SPEEDMULTIADD = 0.5f;

	void Init()override;
	void Release();

	std::shared_ptr<KdModelData>m_model;
	float m_length = 0.0f;
	float m_angleDeg;
	float m_linePos;
	float m_angleDegZ = 0.0f;

	//影対策
	bool m_isShadow = false;

};