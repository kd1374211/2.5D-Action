#include "ScoreManager.h"
#include "../Object/Character/CharaManager.h"
#include "../Object/Character/Player/Player.h"
#include "../Scene/SceneManager.h"

void ScoreManager::Reset()
{
	m_currentHeight = 0.0f;
	m_currentKill = 0;
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

void ScoreManager::Init()
{}

void ScoreManager::Release()
{}
