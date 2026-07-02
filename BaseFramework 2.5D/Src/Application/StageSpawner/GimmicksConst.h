#pragma once

static const int MAXLEVEL = 10;

enum class Gimmicks
{
	Boulder,
	Wood_05,
	Wood_10,
	Wood_15,
	Wood_20,
	Spear_Three,
	Spear_Five,
	SpearAfterSide,
	SideSpear,
	Goblin,
	FlyEye,
	Max
};

struct GimmicksData
{
	int m_minLevel;
	float m_linePosMin;
	float m_linePosMax;
	int m_chance[MAXLEVEL];
};

struct NoStairsData
{
	int m_chance;
	int m_cool;
	int m_tripleChance;
};