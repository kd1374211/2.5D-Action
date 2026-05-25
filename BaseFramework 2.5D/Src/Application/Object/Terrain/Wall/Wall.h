#pragma once

class Wall :public KdGameObject
{
public:

	Wall(Math::Vector3 a_startPos);
	~Wall()override { Release(); }

	void Update()override;
	void DrawUnLit()override;
	void GenerateDepthMapFromLight()override;

private:

	void Init()override;
	void Release();

	std::shared_ptr<KdModelData> m_model;
	Math::Vector3 m_pos;

};