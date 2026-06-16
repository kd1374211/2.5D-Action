#pragma once
#include "../EnemyBase.h"

class Goblin :public EnemyBase
{
public:

	Goblin(Math::Vector3 a_startPos, float a_startDeg, float a_linePos);
	~Goblin()override { Release(); }

	void Update()override;

private:

	void Init()override;
	void Release();

};