#pragma once

class HeartChargeBar :public KdGameObject
{
public:

	HeartChargeBar() { Init(); }
	~HeartChargeBar()override { Release(); }

	void PreDraw()override;
	void DrawSprite()override;

private:

	//滑らかにしたい
	const float CHARGESPEED = 0.01f;

	//落下死
	const float CHARGEREDUCE_FALLVOID = 0.05f;

	//フェードイン
	const float ALPHAADD = 0.05f;

	const Math::Vector2 BASETEXSIZE = Math::Vector2(48.0f, 192.0f);
	const float TEXSCALE = 2.0f;
	const Math::Vector2 TEXDRAWSIZE = BASETEXSIZE * TEXSCALE;
	const Math::Vector2 TEXDRAWPOS = Math::Vector2(-590.0f, 20.0f);

	const float BARBOTTOM = 9.0f;
	const float BARHEIGHT = 135.0f;

	void Init()override;
	void Release() {};

	std::shared_ptr<KdTexture> m_barTex;
	std::shared_ptr<KdTexture> m_barTex_reduce;
	std::shared_ptr<KdTexture> m_frameTex;
	
	float m_heartBarFill = 1.0f;
};