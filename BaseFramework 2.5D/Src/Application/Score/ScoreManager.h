#pragma once

enum class CompareResult
{
	Less = -1,
	Equal = 0,
	Greater = 1
};

class ScoreManager
{
public:

	//最大桁数
	static const int DIGITS_HEIGHTS = 4;
	static const int DIGITS_KILLS = 3;

	//スコアリセット
	void Reset();

	//セッター（アッダー）
	void AddKillCount();
	void AddHeight(float a_height);

	//ゲッターs
	int GetCurrentHeight()const { return (int)m_currentHeight; }
	int GetCurrentKillCount()const { return m_currentKill; }

	//スコアの比較
	CompareResult CompareNum(int a_baseNum, int a_targetNum);

	//桁分解
	int CalcDigit(int a_baseNumber, int retDigit);

private:

	ScoreManager() { Init(); }
	~ScoreManager() { Release(); }

	void Init();
	void Release();

	//最大値
	const float HEIGHTS_MAX = 9999.9f;
	const int KILLS_MAX = 999;

	float m_currentHeight = 0.0f;
	int m_currentKill = 0;

public:

	static ScoreManager& Instance()
	{
		static ScoreManager instance;
		return instance;
	}

};

#define SCOREMGR ScoreManager::Instance()