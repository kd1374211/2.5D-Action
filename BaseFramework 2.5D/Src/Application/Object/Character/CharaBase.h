#pragma once
#include "CharaManagerConst.h"

class CharaBase :public KdGameObject
{
public:

	CharaBase() {}
	~CharaBase()override {}

	bool GetIsDead()const { return m_isDead; }

protected:

	bool m_isDead = false;

};