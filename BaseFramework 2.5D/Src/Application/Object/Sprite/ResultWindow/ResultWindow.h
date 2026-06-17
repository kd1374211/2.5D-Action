#pragma once

class ResultWindow :public KdGameObject
{
public:

	ResultWindow() { Init(); }
	~ResultWindow()override { Release(); }

	void Update()override;
	void DrawSprite()override;

private:

	void Init()override;
	void Release();

};