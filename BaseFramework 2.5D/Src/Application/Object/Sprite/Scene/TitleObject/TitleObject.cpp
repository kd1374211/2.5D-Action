#include "TitleObject.h"

void TitleObject::Update()
{
	if (m_isFade)
	{
		m_alpha -= FADEOUTSPEED;
		if (m_alpha <= 0.0f)
		{
			m_alpha = 0.0f;
			m_isFadeEnd = true;
		}
	}
}

void TitleObject::DrawSprite()
{
	//透明度用
	Math::Color color = { 1.0f, 1.0f, 1.0f, m_alpha };

	//タイトル画像
	Math::Rectangle rec = { 0, 0, (long)m_titleTex->GetInfo().Width, (long)m_titleTex->GetInfo().Height };
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_titleTex, TEXDRAWPOS_TITLE.x, TEXDRAWPOS_TITLE.y, m_titleTex->GetInfo().Width, m_titleTex->GetInfo().Height, &rec, &color);

	//PressSpace
	rec = { 0, 0, (long)m_pressSpaceTex->GetInfo().Width, (long)m_pressSpaceTex->GetInfo().Height };
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pressSpaceTex, TEXDRAWPOS_PRESSSPACE.x, TEXDRAWPOS_PRESSSPACE.y, m_pressSpaceTex->GetInfo().Width, m_pressSpaceTex->GetInfo().Height, &rec, &color);
}

void TitleObject::Init()
{
	//タイトル本体
	m_titleTex = std::make_shared<KdTexture>();
	m_titleTex->Load("Asset/Textures/Scene/Title/TitleLogo.png");

	//PressSpace
	m_pressSpaceTex = std::make_shared<KdTexture>();
	m_pressSpaceTex->Load("Asset/Textures/Scene/Title/PressSpace.png");
}

void TitleObject::Release()
{}
