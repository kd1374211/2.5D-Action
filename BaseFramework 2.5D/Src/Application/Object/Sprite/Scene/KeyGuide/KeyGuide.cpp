#include "KeyGuide.h"
#include "../../../../Fonts/FontsManager.h"

void KeyGuide::PreDraw()
{
	//アルファ更新
	if (m_alpha < 1.0f)
	{
		m_alpha += ALPHAADD;
		if (m_alpha >= 1.0f)m_alpha = 1.0f;
	}
}

void KeyGuide::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	Math::Color color = kWhiteColor;
	color.w = m_alpha;

	for (auto& itr : FONTMGR.GetTextData(TextScene::Game_KeyGuide))
	{
		KdShaderManager::Instance().m_spriteShader.DrawFont(itr.m_type, itr.m_pos, itr.m_base, &color, itr.m_str.c_str());
	}
}

void KeyGuide::Init()
{}

void KeyGuide::Release()
{}
