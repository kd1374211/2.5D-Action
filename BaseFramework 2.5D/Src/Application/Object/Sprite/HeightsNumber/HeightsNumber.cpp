#include "HeightsNumber.h"
#include "../../../Scene/SceneManager.h"
#include "../../Character/Player/Player.h"
#include "../../Character/CharaManager.h"

void HeightsNumber::Update()
{
	//プレイヤーが死んでいたらスキップ
	if (CHARAMGR.GetPlayer()->GetIsDead())return;

	float scroll = SCENEMGR.GetScrollSpeedMulti();
	SCOREMGR.AddHeight(scroll * 0.05f);
}

void HeightsNumber::PostUpdate()
{
	//プレイヤーが死んでいたらスキップ
	if (CHARAMGR.GetPlayer()->GetIsDead())return;

	float scrollBack = SCENEMGR.GetScrollBack();
	SCOREMGR.AddHeight(-scrollBack * 0.05f);
}

void HeightsNumber::PreDraw()
{
	//数字のスクロール計算
	if (!m_isNumScroll)
	{
		//目標の高さを取得
		int targetHeight = SCOREMGR.GetCurrentHeight();

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

	KdDebugGUI::Instance().AddLog("DrawNum : %d\n", m_drawNum);
}

void HeightsNumber::DrawSprite()
{
	Math::Rectangle rec;

	//アイコン
	rec = Math::Rectangle(0, 0, (long)BASEICONSIZE, (long)BASEICONSIZE);
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_iconTex, -180.0f, 320.0f, ICONSIZE, ICONSIZE, &rec);

	//数字
	//次の高さで変わる数字を取得
	for (int i = SCOREMGR.DIGITS_HEIGHTS; i > 0; i--)
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
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_numberTex, 192 - i * NUMSIZE, 320, NUMSIZE, NUMSIZE, &rec);
	}
}

void HeightsNumber::Init()
{
	m_numberTex = std::make_shared<KdTexture>();
	m_numberTex->Load("Asset/Textures/Scene/Game/Number.png");

	m_iconTex = std::make_shared<KdTexture>();
	m_iconTex->Load("Asset/Textures/Scene/Game/HeightIcon.png");
}

void HeightsNumber::Release()
{}
