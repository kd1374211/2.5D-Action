#include "KillCount.h"

void KillCount::PostUpdate()
{
	//星の数更新
	if (m_currentStar < SCOREMGR.GetKillsRank())
	{
		m_currentStar++;
		m_starSize.push_back(0.0f);
	}

	//星のサイズ更新
	for (auto& star : m_starSize)
	{
		if (star < STARSIZEMAX)star += STARSIZEADD;
		if (star >= STARSIZEMAX)star = STARSIZEMAX;
	}
}

void KillCount::PreDraw()
{
	//アルファ更新
	if (m_alpha < 1.0f)
	{
		m_alpha += ALPHAADD;
		if (m_alpha >= 1.0f)m_alpha = 1.0f;
	}

	if (!m_isNumScroll)
	{
		//目標の高さを取得
		int targetHeight = SCOREMGR.GetCurrentKillCount();

		//スクロール方向を設定
		m_scrollDir = SCOREMGR.CompareNum(m_drawNum, targetHeight);

		//Equalならリターン
		if (m_scrollDir == CompareResult::Equal)return;

		//スクロールオン
		m_isNumScroll = true;
		m_numScroll = 0.0f;
	}
	else
	{
		//スクロール
		m_numScroll += SCROLLSPEED * (int)m_scrollDir;

		//次の数字
		if (m_numScroll >= 1.0f)
		{
			m_drawNum++;
			m_isNumScroll = false;
		}
		//前の数字
		else if (m_numScroll <= -1.0f)
		{
			m_drawNum--;
			m_isNumScroll = false;
		}
	}
}

void KillCount::DrawSprite()
{
	Math::Rectangle rec;
	//色
	Math::Color color = Math::Color(1.0f, 1.0f, 1.0f, m_alpha);
	
	//アイコン
	rec = Math::Rectangle(0, 0, (long)BASEICONSIZE, (long)BASEICONSIZE);
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_iconTex, 350.0f, 320.0f, ICONSIZE, ICONSIZE, &rec, &color);
	
	//数字
	//次の高さで変わる数字を取得
	for (int i = SCOREMGR.DIGITS_KILLS; i > 0; i--)
	{
		int digit = (int)pow(10, i - 1);
		int number = SCOREMGR.CalcDigit(m_drawNum, digit);

		//スクロール確認
		if (m_isNumScroll)
		{
			int nextNum = SCOREMGR.CalcDigit(m_drawNum + (int)m_scrollDir, digit);

			if (number != nextNum)
			{
				rec = Math::Rectangle(0, (long)(BASENUMSIZE * (number + m_numScroll)), (long)BASENUMSIZE, (long)BASENUMSIZE);
			}
			else
			{
				rec = Math::Rectangle(0, (long)(BASENUMSIZE * number), (long)BASENUMSIZE, (long)BASENUMSIZE);
			}
		}
		else
		{
			rec = Math::Rectangle(0, (long)(BASENUMSIZE * number), (long)BASENUMSIZE, (long)BASENUMSIZE);
		}

		KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_numTex, 635.0f - i * NUMSIZE, 320.0f, NUMSIZE, NUMSIZE, &rec, &color);
	}

	//星
	Math::Vector2 drawPos = STARDRAWCENTERPOS - Math::Vector2(STARBASESIZE.x * (m_currentStar - 1) / 2.0f, 0.0f);
	rec = Math::Rectangle(0, 0, (long)STARBASESIZE.x, (long)STARBASESIZE.y);
	for (auto& star : m_starSize)
	{
		//サイズ
		Math::Vector2 drawSize = STARBASESIZE * star;

		//描画
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_starTex, drawPos.x, drawPos.y, drawSize.x, drawSize.y, &rec);

		//位置更新
		drawPos.x += STARBASESIZE.x;
	}
}

void KillCount::Init()
{
	m_numTex = std::make_shared<KdTexture>();
	m_numTex->Load("Asset/Textures/Scene/Game/Number.png");

	m_iconTex = std::make_shared<KdTexture>();
	m_iconTex->Load("Asset/Textures/Scene/Game/KillsIcon.png");

	m_starTex = std::make_shared<KdTexture>();
	m_starTex->Load("Asset/Textures/Scene/Game/KillStar.png");
}

void KillCount::Release()
{}
