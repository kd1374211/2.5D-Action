#include "ScoreManager.h"
#include "../Object/Character/CharaManager.h"
#include "../Object/Character/Player/Player.h"
#include "../Scene/SceneManager.h"

void ScoreManager::Update()
{
	//0未満にしない(kill数は0未満にならない)
	if (m_currentHeight < 0.0f)m_currentHeight = 0.0f;
}

void ScoreManager::Reset()
{
	m_currentHeight = 0.0f;
	m_currentKill = 0;
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
