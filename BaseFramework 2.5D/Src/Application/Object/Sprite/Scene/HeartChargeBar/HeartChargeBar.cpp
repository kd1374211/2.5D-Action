#include "HeartChargeBar.h"
#include "../../../../HeartCharge/HeartCharge.h"

void HeartChargeBar::PreDraw()
{
	//アルファ更新
	if (m_alpha < 1.0f)
	{
		m_alpha += ALPHAADD;
		if (m_alpha >= 1.0f)m_alpha = 1.0f;
	}

	//バー更新
	float chargeProg = HEARTCHARGE.GetChargeProgress();
	float chargeDiff = chargeProg - m_heartBarFill;
	if (HEARTCHARGE.GetIsHeartSpawned())
	{
		float move = std::clamp(chargeDiff, -CHARGEREDUCE_AFTERHEART, CHARGESPEED);

		//フラグ消し
		if (move > -CHARGEREDUCE_AFTERHEART)HEARTCHARGE.SetHeartSpawned(false);

		m_heartBarFill += move;
	}
	else
	{
		m_heartBarFill += std::clamp(chargeDiff, -CHARGESPEED, CHARGESPEED);
	}
}

void HeartChargeBar::DrawSprite()
{
	//透明度設定
	Math::Color color = Math::Color(1.0f, 1.0f, 1.0f, m_alpha);

	//フレーム
	Math::Rectangle rec = Math::Rectangle(0, 0, (long)BASETEXSIZE.x, (long)BASETEXSIZE.y);
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_frameTex, TEXDRAWPOS.x, TEXDRAWPOS.y, TEXDRAWSIZE.x, TEXDRAWSIZE.y, &rec, &color);

	//バー
	float chargeBarHeight = BARHEIGHT * m_heartBarFill;
	//最大チャージ
	rec = Math::Rectangle(0, (long)(BASETEXSIZE.y - (BARBOTTOM + chargeBarHeight)), (long)BASETEXSIZE.x, (long)(BARBOTTOM + chargeBarHeight));
	Math::Vector2 drawPos = TEXDRAWPOS - Math::Vector2(0, (BASETEXSIZE.y - BARBOTTOM - chargeBarHeight) / 2.0f) * TEXSCALE;
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_barTex, drawPos.x, drawPos.y, TEXDRAWSIZE.x, (BARBOTTOM + chargeBarHeight) * TEXSCALE, &rec, &color);

	//最大チャージハート
	if (HEARTCHARGE.GetIsMaxCharge())
	{
		rec = Math::Rectangle(0, 0, (long)BASETEXSIZE.x, (long)BASETEXSIZE.y);
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_fullTex, TEXDRAWPOS.x, TEXDRAWPOS.y, TEXDRAWSIZE.x, TEXDRAWSIZE.y, &rec, &color);
	}
}

void HeartChargeBar::Init()
{
	//画像ロード
	m_frameTex = std::make_shared<KdTexture>();
	m_frameTex->Load("Asset/Textures/Heart/HealthBarFrame.png");

	m_barTex = std::make_shared<KdTexture>();
	m_barTex->Load("Asset/Textures/Heart/HealthBar.png");

	m_fullTex = std::make_shared<KdTexture>();
	m_fullTex->Load("Asset/Textures/Heart/FullCharge.png");
}
