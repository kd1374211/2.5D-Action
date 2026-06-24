#pragma once

class KeyGuide :public KdGameObject
{
public:

	KeyGuide() { Init(); }
	~KeyGuide() { Release(); }

	void PreDraw()override;
	void DrawSprite()override;

private:

	void Init()override;
	void Release();

	//フェードイン
	const float ALPHAADD = 0.1f;
	float m_alpha = 0.0f;

};