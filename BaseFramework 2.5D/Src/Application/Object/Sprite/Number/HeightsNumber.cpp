#include "HeightsNumber.h"
#include "../../../Scene/SceneManager.h"

void HeightsNumber::Update()
{
	float scroll = SCENEMGR.GetScrollSpeedMulti();
	m_nowHeight += scroll * 0.05f;
}

void HeightsNumber::PostUpdate()
{
	float scrollBack = SCENEMGR.GetScrollBack();
	m_nowHeight -= scrollBack * 0.05f;
}

void HeightsNumber::DrawSprite()
{
	//整数化＆値コピー
	int height = (int)m_nowHeight;
	int nextHeight = height + 1;

	//少数以下
	float decimal = m_nowHeight - (float)height;

	//次の高さで変わる数字を取得
	for (int i = DIGITS; i > 0; i--)
	{
		int digit = (int)pow(10, i - 1);
		int number = CalcDigit(height, digit);
		int nextNumber = CalcDigit(nextHeight, digit);

		Math::Rectangle rec;

		if (number != nextNumber)
		{
			rec = Math::Rectangle(0, (long)(BASENUMSIZE * (number + decimal)), (long)BASENUMSIZE, (long)BASENUMSIZE);
		}
		else
		{
			rec = Math::Rectangle(0, (long)(BASENUMSIZE * number), (long)BASENUMSIZE, (long)BASENUMSIZE);
		}

		KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, 192 - i * NUMSIZE, 320, NUMSIZE, NUMSIZE, &rec);
	}
}

void HeightsNumber::Init()
{
	m_nowHeight = 0.0f;

	m_tex = std::make_shared<KdTexture>();
	m_tex->Load("Asset/Textures/Number.png");
}

void HeightsNumber::Release()
{}

int HeightsNumber::CalcDigit(int a_baseNumber, int retDigit)
{
	//返還位で割る
	a_baseNumber /= retDigit;

	//より上の位をカット
	a_baseNumber %= 10;

	return a_baseNumber;
}
