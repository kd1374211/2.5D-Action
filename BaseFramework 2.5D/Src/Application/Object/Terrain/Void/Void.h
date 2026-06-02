#pragma once

class Void :public KdGameObject
{
public:

	Void() { Init(); }
	~Void() { Release(); }

	void DrawUnLit()override;

private:

	void Init()override;
	void Release();

	std::shared_ptr<KdModelData> m_model;

};