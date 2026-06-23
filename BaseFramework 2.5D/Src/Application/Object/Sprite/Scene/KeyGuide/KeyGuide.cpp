#include "KeyGuide.h"
#include "../../../../Fonts/FontsManager.h"

void KeyGuide::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	for (auto& itr : FONTMGR.GetTextData(TextScene::Game_KeyGuide))
	{
		KdShaderManager::Instance().m_spriteShader.DrawFont(itr.m_type, itr.m_pos, itr.m_base, &kBlackColor, itr.m_str.c_str());
	}
}

void KeyGuide::Init()
{}

void KeyGuide::Release()
{}
