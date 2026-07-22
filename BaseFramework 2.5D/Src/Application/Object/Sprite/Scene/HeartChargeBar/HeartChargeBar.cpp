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
	float chargeProg = HEARTCHARGE.GetHealth();
	float chargeDiff = chargeProg - m_heartBarFill;
	float move = 0.0f;
	//落下死
	if (HEARTCHARGE.GetIsFallVoid())
	{
		move = std::clamp(chargeDiff, -CHARGEREDUCE_FALLVOID, CHARGESPEED);

		//落下死フラグ消し
		if (move > -CHARGEREDUCE_FALLVOID)HEARTCHARGE.SetIsFallVoid(false);
	}
	else
	{
		move = std::clamp(chargeDiff, -CHARGESPEED, CHARGESPEED);

		//被弾後フラグ消し
		if (HEARTCHARGE.GetIsAfterHit())
		{
			if (move > -CHARGESPEED)HEARTCHARGE.SetIsAfterHit(false);
		}
	}

	m_heartBarFill += move;
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

	//減少チェック(瀕死状態時も)
	if (HEARTCHARGE.GetIsAfterHit() || HEARTCHARGE.GetIsFallVoid() || HEARTCHARGE.GetIsCriticalHealth())
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_barTex_reduce, drawPos.x, drawPos.y, TEXDRAWSIZE.x, (BARBOTTOM + chargeBarHeight) * TEXSCALE, &rec, &color);
	}
	else
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_barTex, drawPos.x, drawPos.y, TEXDRAWSIZE.x, (BARBOTTOM + chargeBarHeight) * TEXSCALE, &rec, &color);
	}
}

void HeartChargeBar::Init()
{
	//画像ロード
	m_frameTex = std::make_shared<KdTexture>();
	m_frameTex->Load("Asset/Textures/Heart/HealthBarFrame.png");

	m_barTex = std::make_shared<KdTexture>();
	m_barTex->Load("Asset/Textures/Heart/HealthBar.png");

	m_barTex_reduce = std::make_shared<KdTexture>();
	m_barTex_reduce->Load("Asset/Textures/Heart/HealthBar_Reduce.png");
}
