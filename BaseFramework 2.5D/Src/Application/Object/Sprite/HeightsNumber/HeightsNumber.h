#pragma once
#include "../../../Score/ScoreManager.h"

class HeightsNumber :public KdGameObject
{
public:

	HeightsNumber() { Init(); }
	~HeightsNumber()override { Release(); }

	void Update()override;
	void PostUpdate()override;
	void PreDraw()override;
	void DrawSprite()override;

private:

	void Init()override;
	void Release();

	//数字画像
	const float BASENUMSIZE = 12.0f;
	const float NUMSIZE = 72.0f;

	//アイコン画像
	const float BASEICONSIZE = 64.0f;
	const float ICONSIZE = 96.0f;

	//数字のスクロール
	const float SCROLLSPEED = 0.25f;
	bool m_isNumScroll = false;
	CompareResult m_scrollDir = CompareResult::Equal;
	float m_numScroll = 0.0f;
	int m_drawNum = 0;

	std::shared_ptr<KdTexture> m_iconTex = nullptr;
	std::shared_ptr<KdTexture> m_numberTex = nullptr;
	std::shared_ptr<KdTexture> m_meterTex = nullptr;
};
