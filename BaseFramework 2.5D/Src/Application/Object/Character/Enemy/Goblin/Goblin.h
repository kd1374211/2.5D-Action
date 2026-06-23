#pragma once
#include "../EnemyBase.h"

class Goblin :public EnemyBase
{
public:

	Goblin(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos);
	~Goblin()override { Release(); }

	void Update()override;

private:

	//出現位置
	const Math::Vector3 SPAWNPOS = Math::Vector3(0, 0.24f, 0);

	//球判定Pos
	const Math::Vector3 SPHEREHITOFS = Math::Vector3(0, 0.6f, 0);

	void Init()override;
	void Release();

};