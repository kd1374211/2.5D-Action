#pragma once
#include "CharaManagerConst.h"

class CharaBase :public KdGameObject
{
public:

	CharaBase() {}
	~CharaBase()override {}

	bool GetIsDead()const { return m_isDead; }

protected:

	//重力
	const float GRAVITY = 0.01f;
	const float FALLSPEEDMAX = -0.4f;
	float m_moveY = 0.0f;

	float m_animCnt = 0.0f;
	bool m_isDead = false;
	
	//影対策
	bool m_isShadow = false;

};