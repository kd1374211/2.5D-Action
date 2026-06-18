#include "KillCount.h"

void KillCount::Update()
{}

void KillCount::DrawSprite()
{

}

void KillCount::Init()
{
	m_numTex = std::make_shared<KdTexture>();
	m_numTex->Load("Assets/Texture/Scene/Game/Number.png");

	m_skullIconTex = std::make_shared<KdTexture>();
	m_skullIconTex->Load("Assets/Texture/Scene/Game/SkullIcon.png");

}

void KillCount::Release()
{}
