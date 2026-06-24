#pragma once

static const int MAXLEVEL = 10;

enum class Gimmicks
{
	Boulder,
	Wood_05,
	Wood_10,
	Wood_15,
	Wood_20,
	Spear,
	SpearAfterSide,
	SideSpear,
	Goblin,
	FlyEye,
	Max
};

struct GimmicksData
{
	float m_linePosMin;
	float m_linePosMax;
	int m_minLevel;
	int m_chance[MAXLEVEL];
};
