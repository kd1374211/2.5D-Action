#pragma once
#include "../EnemyBase.h"

class FlyEye :public EnemyBase
{
public:

	FlyEye(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos);
	~FlyEye()override { Release(); }

	void Update()override;
	
private:

	//出現位置
	const Math::Vector3 SPAWNPOS = Math::Vector3(0, 2.0f, 0);

	//球判定Pos
	const Math::Vector3 SPHEREHITOFS = Math::Vector3::Zero;

	void Init()override;
	void Release();

};