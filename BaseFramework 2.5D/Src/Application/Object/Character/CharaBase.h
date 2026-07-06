#pragma once
#include "CharaManagerConst.h"

class CharaBase :public KdGameObject
{
public:

	CharaBase() {}
	~CharaBase()override {}

	bool GetIsDead()const { return m_isDead; }

protected:

	float m_animCnt = 0.0f;
	bool m_isDead = false;
	
	//影対策
	bool m_isShadow = false;

};