#pragma once

enum PlayerAnimType
{
	Run,
	Jump,
	Attack,
	Hit,
	Dead,
	Number
};

struct PolygonData
{
	float m_animMax;
	float m_animAdd;
	std::shared_ptr<KdSquarePolygon> m_polygon;
};
