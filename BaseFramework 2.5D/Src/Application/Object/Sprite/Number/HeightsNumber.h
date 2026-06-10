#pragma once

class HeightsNumber :public KdGameObject
{
public:

	HeightsNumber() { Init(); }
	~HeightsNumber()override { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawSprite()override;

private:

	static const int DIGITS = 3;
	const float BASENUMSIZE = 12.0f;
	const float NUMSIZE = 96.0f;

	void Init()override;
	void Release();

	//桁分解
	int CalcDigit(int a_baseNumber, int retDigit);

	//現在の高さ
	float m_nowHeight;

	std::shared_ptr<KdTexture> m_tex;

};