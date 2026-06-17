#pragma once

class Transition :public KdGameObject
{
public:

	Transition() { Init(); }
	~Transition()override { Release(); }

	void Update()override;
	void DrawSprite()override;

	//ゲッター
	bool GetIsReset()const { return m_isReset; }

private:

	void Init()override;
	void Release();

	std::shared_ptr<KdTexture> m_tex = nullptr;

	bool m_isReset = false;

};