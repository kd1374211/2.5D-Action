#pragma once

class Spear :public KdGameObject
{
public:

	Spear(Math::Vector3 a_startPos, float a_startDeg, float a_linePos);
	~Spear()override { Release(); }

	void Update()override;
	void DrawLit()override;

private:

	void Init()override;
	void Release();

	//位置
	Math::Vector3 m_pos;

	//槍の位置
	const float SPEARPOSMAX = 5.0f;
	const float SPEARPOSSTART = -5.0f;
	const float SPEARMOVEY = 1.0f;

	float m_spearPos = SPEARPOSSTART;

	//螺旋位置
	float m_linePos;

	//角度
	float m_angleDeg;

	//槍の穴専用
	Math::Matrix m_mHoleWorld;

	//槍出現フラグ
	const float SPEARACTIVEY = 4.0f;

	bool m_isSpear = false;

	//モデル2個
	std::shared_ptr<KdModelData> m_holeModel;
	std::shared_ptr<KdModelData> m_spearModel;

};