#pragma once

enum class PlayerAnimType
{
	Run,
	Jump,
	Fall,
	Attack,
	Hit,
	Dead,
	PlayerAnimNumber
};

enum EnemyName
{
	EnemyName_Goblin,
	EnemyName_FlyEye,
	EnemyNumber
};

enum class EnemyAnimType
{
	Idle,
	Hit,
	EnemyAnimNumber
};

struct PolygonData
{
	float m_animMax;
	float m_animAdd;
	std::shared_ptr<KdSquarePolygon> m_polygon;
};
