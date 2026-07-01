#include "ScoreManager.h"
#include "../Object/Character/CharaManager.h"
#include "../Object/Character/Player/Player.h"
#include "../Scene/SceneManager.h"

void ScoreManager::Reset()
{
	m_currentHeight = 0.0f;
	m_currentKill = 0;
	SetCountStart(false);
}

void ScoreManager::AddKillCount()
{
	m_currentKill++;
	if (m_currentKill > KILLS_MAX)m_currentKill = KILLS_MAX;
}

void ScoreManager::AddHeight(float a_height)
{
	m_currentHeight += a_height;
	if (m_currentHeight < 0.0f)m_currentHeight = 0.0f;
	else if (m_currentHeight > HEIGHTS_MAX)m_currentHeight = HEIGHTS_MAX;
}

CompareResult ScoreManager::CompareNum(int a_baseNum, int a_targetNum)
{
	if (a_baseNum == a_targetNum)return CompareResult::Equal;
	else return a_baseNum < a_targetNum ? CompareResult::Greater : CompareResult::Less;
}

int ScoreManager::CalcDigit(int a_baseNumber, int retDigit)
{
	//返還位で割る
	a_baseNumber /= retDigit;

	//より上の位をカット
	a_baseNumber %= 10;

	return a_baseNumber;
}

std::string ScoreManager::RandRankText()
{
	//ランダム(伝説は出ない)
	int first = rand() % (RANKS - 1);
	int second = rand() % RankCompare::Number;

	//ランクテキスト生成
	std::string rankText = m_rankTextData_First[first].m_rankName + m_rankTextData_Second[second];

	return rankText;
}

std::string ScoreManager::GetRankText()
{
	int heightRank = 0;
	int killRank = 0;
	int totalRank = 0;

	//ランク計算
	for (int i = 0; i < RANKS; i++)
	{
		//超えている間は増やす(それに上書きする)
		if (m_currentHeight > m_rankTextData_First[i].m_rankTarget_Height)
		{
			heightRank = i;
		}

		//超えている間は増やす(それに上書きする)
		if (m_currentKill > m_rankTextData_First[i].m_rankTarget_Kills)
		{
			killRank = i;
		}
	}

	RankCompare compare = RankCompare::Number;

	//ランク比べ
	if (heightRank > killRank)
	{
		compare = RankCompare::Height;
		totalRank = heightRank;
	}
	else if (heightRank < killRank)
	{
		compare = RankCompare::Kills;
		totalRank = killRank;
	}
	else
	{
		compare = RankCompare::Balance;
		totalRank = heightRank;
	}

	//ランクテキスト生成
	std::string rankText = m_rankTextData_First[totalRank].m_rankName + m_rankTextData_Second[compare];

	return rankText;
}

void ScoreManager::Init()
{
	LoadData();
}

void ScoreManager::LoadData()
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, "Asset/Data/Scene/RankScoreTargetData.csv", "r") == 0)
	{
		const int STRLENG = 250;
		char dummy[STRLENG] = {};
		int index = 0;
		char text[STRLENG] = {};

		RankData data;
		
		//上の句
		for (int i = 0; i < RANKS; i++)
		{
			if (fgets(dummy, STRLENG, fp) != nullptr)
			{
				fscanf_s(fp, "%d,%[^,],%f,%d,",
					&index,
					text, STRLENG,
					&data.m_rankTarget_Height,
					&data.m_rankTarget_Kills);

				data.m_rankName = text;
				m_rankTextData_First.push_back(data);
			}
		}

		//1行読み
		fgets(dummy, STRLENG, fp);

		//下の句
		for (int i = 0; i < RankCompare::Number; i++)
		{
			if (fgets(dummy, STRLENG, fp) != nullptr)
			{
				fscanf_s(fp, "%d,%[^,],",
					&index,
					text, STRLENG);

				m_rankTextData_Second.push_back(text);
			}
		}

		fclose(fp);
	}
}

void ScoreManager::Release()
{}
