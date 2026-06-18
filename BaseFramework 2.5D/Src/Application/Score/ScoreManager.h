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

	void Update();
	void Reset();

	//セッター（アッダー）
	void AddKillCount() { m_currentKill++; }
	void AddHeight(float a_height) { m_currentHeight += a_height; }

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