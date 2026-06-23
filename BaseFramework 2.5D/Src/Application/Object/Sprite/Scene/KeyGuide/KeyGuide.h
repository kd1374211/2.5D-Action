#pragma once

class KeyGuide :public KdGameObject
{
public:

	KeyGuide() { Init(); }
	~KeyGuide() { Release(); }

	void DrawSprite()override;

private:

	void Init()override;
	void Release();

};