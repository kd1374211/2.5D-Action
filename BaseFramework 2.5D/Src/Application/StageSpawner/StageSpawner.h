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

	void RandGimmicks();

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
	static const int GIMMICKSTORE = 20;
	std::map<Gimmicks, GimmicksData> m_gimmicksData;
	std::list<SpawnData> m_spawnData;

	//壁
	std::shared_ptr<Wall> m_wall;
	
	//柱
	std::shared_ptr<Pillar> m_pillar;

	//上に飛んだ階段のカウンター（これが０じゃない間は障害がスポーンしない）
	int m_countBackScroll = 0;

	//横槍
	bool m_isSideSpearNext = false;

	//非出現フラグ
	bool noSpawnFlg = false;
	int noSpawnStairCnt = 0;

public:

	static StageSpawner& Instance()
	{
		static StageSpawner instance;
		return instance;
	}

};

#define STAGESPAWNER StageSpawner::Instance()