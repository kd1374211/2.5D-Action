#pragma once
#include "GimmicksConst.h"

class Stair;
class Wall;
class Pillar;
class BaseScene;

class StageSpawner
{
public:

	void StartGame(BaseScene* a_scene);
	void ResetStage();
	void Update();

	//デバッグ
	int GetStairStore()const { return m_countBackScroll; }

	//仮
	void AddPastStairVisibleLog(bool a_isVisible);
	void AddFutureStairVisibleLog(bool a_isVisible);
	std::list<bool> GetStoreList_P() { return m_stairVisibleLog_past; }
	std::list<bool> GetStoreList_F() { return m_stairVisibleLog_future; }

private:

	StageSpawner() { Init(); }
	~StageSpawner() { Release(); }

	void Init();
	void LoadData();
	void Release();

	//同時に存在する階段数
	static const int STAIRNUM = 80;
	static const int PILLARWALLNUM = 3;

	//角度関連
	const float STAIRDEGBASE = 0.0f;
	const float STAIRDEGDIFF = 9.0f;

	//位置関連
	const float STAIRSPAWNRANGE = 5.0f;
	const float STAIRSPAWNBASE_Y = -3.28f;
	const float STAIRSPAWNDIFF_Y = 0.144f;

	//階段80個(後ろにあるほど上にある)
	std::list<std::shared_ptr<Stair>> m_stairs;
	
	//最大貯蔵数
	static const int MAXDATASTORE = 80;
	static const int MAXFUTUREROLL = 40;

	//仮
	std::list <bool> m_stairVisibleLog_past;
	std::list<bool> m_stairVisibleLog_future;

	//敵スポーンデータ
	std::map<Gimmicks, GimmicksData> m_gimmicksData;
	
	//壁
	std::shared_ptr<Wall> m_wall;
	
	//柱
	std::shared_ptr<Pillar> m_pillar;

	//上に飛んだ階段のカウンター（これが０じゃない間は障害がスポーンしない）
	int m_countBackScroll = 0;

	//横槍
	bool m_isSideSpearNext = false;

	//非出現フラグ
	NoStairsData m_noStairsData[MAXLEVEL];
	bool m_noSpawnFlg = false;
	int m_noSpawnStairCnt = 0;
	int m_noSpawnStairCool = 0;

	//ギミック連続出現対策
	static const int BOULDERCOOL = 4;
	static const int WOODCOOL = 2;
	static const int WOODRUSHLIMIT = 2;
	int m_boulderCool = 0;
	int m_woodRushCnt = 0;
	int m_woodCool = 0;

	//ステージレベル
	const float LEVELHEIGHTS[MAXLEVEL] = { 0,50.0f,100.0f,150.0f,200.0f,250.0f,300.0f,350.0f,400.0f,450.0f };
	int m_level = 0;

	//回復ハート
	static const int HEARTSPAWNNUM = 7;
	const float HEARTSPAWN[HEARTSPAWNNUM] = { 25.0f,75.0f,150.0f,225.0f,300.0f,375.0f,475.0f };

	static const int HEARTSPAWNCHANCE = 10;
	int m_heartSpawnStack = 0;
	int m_heartSpawnCnt = 0;

public:

	static StageSpawner& Instance()
	{
		static StageSpawner instance;
		return instance;
	}

};

#define STAGESPAWNER StageSpawner::Instance()