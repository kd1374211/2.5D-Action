#include "ResultStars.h"
#include "../../../../Score/ScoreManager.h"
#include "../../../../Scene/ResultScene/ResultScene.h"
#include "../ResultWindow/ResultWindow.h"

void ResultStars::Update()
{
	if (m_wpOwner.expired())return;

	//オーナー取得
	std::shared_ptr<ResultWindow> owner = m_wpOwner.lock();

	//消滅
	if (owner->IsExpired())m_isExpired = true;
}

void ResultStars::DrawSprite()
{
	if (m_wpOwner.expired())return;

	//オーナー取得
	std::shared_ptr<ResultWindow> owner = m_wpOwner.lock();

	//座標取得
	Math::Vector2 ownerPos = Math::Vector2(owner->GetPos().x, owner->GetPos().y);

	//カウント取得
	int countF = owner->GetCountF();

	if (countF > ResultScene::HEIGHTDRAWF)
	{
		//距離
		for (int i = 0; i < STARNUM[(int)StarIndex::Heights]; i++)
		{
			StarColorType type = i < m_heightRank ? StarColorType::Height : StarColorType::Empty;
			Math::Rectangle rec = Math::Rectangle((long)(TEXBASESIZE.x * type), 0, (long)TEXBASESIZE.x, (long)TEXBASESIZE.y);
			Math::Vector2 drawPos = ownerPos + STARDRAWSTARTPOS[(int)StarIndex::Heights] + Math::Vector2(TEXBASESIZE.x * i, 0);
			KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, drawPos.x, drawPos.y, TEXBASESIZE.x, TEXBASESIZE.y, &rec);
		}
	}

	if (countF > ResultScene::KILLSDRAWF)
	{
		//撃破
		for (int i = 0; i < STARNUM[(int)StarIndex::Kills]; i++)
		{
			StarColorType type = i < m_killRank ? StarColorType::Kill : StarColorType::Empty;
			Math::Rectangle rec = Math::Rectangle((long)(TEXBASESIZE.x * type), 0, (long)TEXBASESIZE.x, (long)TEXBASESIZE.y);
			Math::Vector2 drawPos = ownerPos + STARDRAWSTARTPOS[(int)StarIndex::Kills] + Math::Vector2(TEXBASESIZE.x * i, 0);
			KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, drawPos.x, drawPos.y, TEXBASESIZE.x, TEXBASESIZE.y, &rec);
		}
	}

	if (countF > ResultScene::RANKTEXTDRAWF)
	{
		//トータル
		for (int i = 0; i < STARNUM[(int)StarIndex::Total]; i++)
		{
			//i < Height = Height/Height < i < Height + Kill = Kill/Height + Kill < i = Empty
			StarColorType type = StarColorType::Empty;
			if (i < m_heightRank)type = StarColorType::Height;
			else if (i < m_heightRank + m_killRank)type = StarColorType::Kill;
			else type = StarColorType::Empty;

			//パーフェクト
			if (m_heightRank + m_killRank == 10)type = StarColorType::Gold;

			Math::Rectangle rec = Math::Rectangle((long)(TEXBASESIZE.x * type), 0, (long)TEXBASESIZE.x, (long)TEXBASESIZE.y);
			Math::Vector2 drawPos = ownerPos + STARDRAWSTARTPOS[(int)StarIndex::Total] + Math::Vector2(TEXBASESIZE.x * i, 0);
			KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, drawPos.x, drawPos.y, TEXBASESIZE.x, TEXBASESIZE.y, &rec);
		}
	}
}

void ResultStars::Init()
{
	m_heightRank = SCOREMGR.GetHeightRank();
	m_killRank = SCOREMGR.GetKillsRank();

	m_tex = std::make_shared<KdTexture>();
	m_tex->Load("Asset/Textures/Scene/Result/ResultStars.png");
}
