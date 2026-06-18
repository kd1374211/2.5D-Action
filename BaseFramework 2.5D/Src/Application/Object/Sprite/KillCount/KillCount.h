#pragma once
#include "../../../Score/ScoreManager.h"

class KillCount :public KdGameObject
{
public:

	KillCount() { Init(); }
	~KillCount()override { Release(); }

	void Update()override;
	void DrawSprite()override;

private:

	void Init()override;
	void Release();

	static const int DIGITS = 3;
	const float BASENUMSIZE = 12.0f;
	const float NUMSIZE = 96.0f;

	//数字のスクロール速度
	const float SCROLLSPEED = 0.25f;
	float m_numScroll = 0.0f;

	std::shared_ptr<KdTexture> m_skullIconTex = nullptr;
	std::shared_ptr<KdTexture> m_numTex = nullptr;

	int m_lastKills = 0;

};