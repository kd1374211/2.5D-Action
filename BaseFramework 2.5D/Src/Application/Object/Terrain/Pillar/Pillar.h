#pragma once

class Pillar :public KdGameObject
{
public:

	Pillar(Math::Vector3 a_startPos);
	~Pillar()override { Release(); }

	void Update()override;
	void DrawUnLit()override;
	void GenerateDepthMapFromLight()override;

private:

	void Init()override;
	void Release();

	Math::Vector3 m_pos;
	std::shared_ptr<KdModelData> m_model;

};