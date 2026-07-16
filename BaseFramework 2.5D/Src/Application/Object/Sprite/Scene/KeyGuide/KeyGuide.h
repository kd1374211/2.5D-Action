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
	const float ALPHAADD = 0.05f;
};