#pragma once
#include "GimmicksConst.h"

class Stair;
class Wall;
class Pillar;
class BaseScene;
class EnemyBase;

class StageSpawner
{
public:

	void StartGame(BaseScene* a_scene);
	void ResetStage();
	void Update();

	//階段データ
	void AddPastStairVisibleLog(bool a_isVisible);
	void AddFutureStairVisibleLog(bool a_isVisible);
	std::list<bool> GetStoreList_P() { return m_stairVisibleLog_past; }
	std::list<bool> GetStoreList_F() { return m_stairVisibleLog_future; }

	//敵死亡
	void OnEnemyDead(int a_enemyID);
	void DeleteEnemyMap(int a_enemyID);
	
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
	std::map<int, std::weak_ptr<Stair>> m_stairsIDmap;

	//最大貯蔵数
	static const int MAXDATASTORE = 80;
	static const int MAXFUTUREROLL = 40;

	//仮
	std::list <bool> m_stairVisibleLog_past;
	std::list<bool> m_stairVisibleLog_future;

	//ギミックスポーンデータ
	std::map<Gimmicks, GimmicksData> m_gimmicksData;
	
	//壁
	std::shared_ptr<Wall> m_wall;
	
	//柱
	std::shared_ptr<Pillar> m_pillar;

	//上に飛んだ階段のカウンター（これが０じゃない間は障害がスポーンしない）
	int m_countBackScroll = 0;

	//横槍
	bool m_isSideSpearNext = false;
	int m_spearWait = 0;

	//非出現フラグ
	NoStairsData m_noStairsData[MAXLEVEL];
	bool m_stairSpawnFlg = true;
	int m_noSpawnStairCnt = 0;
	int m_noSpawnStairCool = 0;

	//ギミック連続出現対策
	static const int BOULDERCOOL = 8;
	static const int BOULDERCOOL_AFTERWOOD = 1;
	static const int WOODCOOL = 2;
	static const int WOODCOOL_AFTERBOULDER = 1;
	static const int SPEARCOOL = 4;
	int m_boulderCool = 0;
	int m_woodCool = 0;

	//ステージレベル
	const float LEVELHEIGHTS[MAXLEVEL] = { 0,50.0f,100.0f,150.0f,200.0f,250.0f,300.0f,400.0f,500.0f,600.0f };
	int m_level = 0;

	//回復ハート
	static const int HEARTFLYMIN = 9;
	static const int HEARTFLYMAX = 15;
	
	static const int HEARTSPAWNCHANCESTART = 10;
	static const int HEARTSPAWNCHANCEADD = 10;
	int m_heartSpawnCnt = 0;
	int m_heartSpawnChance = HEARTSPAWNCHANCESTART;
	int m_heartSpawnCool = 0;

	//敵と階段をIDで結びつけ
	std::map<int, int> m_enemyStairMap;

public:

	static StageSpawner& Instance()
	{
		static StageSpawner instance;
		return instance;
	}

};

#define STAGESPAWNER StageSpawner::Instance()