#pragma once

enum FontType
{
	CP_72,
	CP_48,
	CP_36,
	CP_24,
	FontsMax
};

enum TextScene
{
	Game_KeyGuide,
	Result_ResultWindow,
	Max
};

enum Texts
{
	KeyGuide_Move,
	KeyGuide_Jump,
	KeyGuide_Attack,
	Result_Result,
	Result_Heights,
	Result_HeightsScore,
	Result_Kills,
	Result_KillsScore,
	Result_Rank,
	Result_RankText,
	Result_ReturnTitle,
	Texts_Max
};

struct TextData
{
	Texts m_Id;
	FontType m_type;
	Math::Vector2 m_pos;
	KdSpriteShader::TextDrawBase m_base;
	std::string m_str;
};