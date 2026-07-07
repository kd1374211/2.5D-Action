#pragma once

enum class CompareResult
{
	Less = -1,
	Equal = 0,
	Greater = 1
};

struct RankTextData
{
	Math::Color m_color;
	std::string m_text;
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
	void SetCountStart(bool a_flg) { m_isCountStart = a_flg; }

	//ゲッターs
	int GetCurrentHeight()const { return (int)m_currentHeight; }
	int GetCurrentKillCount()const { return m_currentKill; }
	bool GetCountStartFlg()const { return m_isCountStart; }
	
	//スコアの比較
	CompareResult CompareNum(int a_baseNum, int a_targetNum);

	//桁分解
	int CalcDigit(int a_baseNumber, int retDigit);

	//ランク
	int GetKillsRank();
	int GetHeightRank();

	//ランク名
	std::string RandRankText();
	RankTextData GetRankText();

private:

	static const int RANKS = 6;
	enum RankCompare
	{
		Height,
		Kills,
		Balance,
		Number
	};

	struct RankData
	{
		std::string m_rankName;
		float m_rankTarget_Height;
		int m_rankTarget_Kills;
	};

	ScoreManager() { Init(); }
	~ScoreManager() { Release(); }

	void Init();
	void LoadData();
	void Release();

	//最大値
	const float HEIGHTS_MAX = 9999.9f;
	const int KILLS_MAX = 999;

	float m_currentHeight = 0.0f;
	int m_currentKill = 0;

	//ランク関連
	std::vector<RankData> m_rankTextData_First;
	std::vector<std::string> m_rankTextData_Second;

	//カウント開始フラグ
	bool m_isCountStart = false;

public:

	static ScoreManager& Instance()
	{
		static ScoreManager instance;
		return instance;
	}

};

#define SCOREMGR ScoreManager::Instance()