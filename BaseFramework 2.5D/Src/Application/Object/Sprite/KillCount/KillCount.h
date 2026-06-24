#pragma once
#include "../../../Score/ScoreManager.h"

class KillCount :public KdGameObject
{
public:

	KillCount() { Init(); }
	~KillCount()override { Release(); }

	void PreDraw()override;
	void DrawSprite()override;

private:

	void Init()override;
	void Release();

	const float BASENUMSIZE = 12.0f;
	const float NUMSIZE = 72.0f;

	const float BASEICONSIZE = 64.0f;
	const float ICONSIZE = 96.0f;

	//数字のスクロール速度
	const float SCROLLSPEED = 0.25f;
	bool m_isNumScroll = false;
	CompareResult m_scrollDir = CompareResult::Equal;
	float m_numScroll = 0.0f;
	int m_drawNum = 0;

	//フェードイン
	const float ALPHAADD = 0.1f;
	float m_alpha = 0.0f;

	std::shared_ptr<KdTexture> m_iconTex = nullptr;
	std::shared_ptr<KdTexture> m_numTex = nullptr;
};