#pragma once

class Spear :public KdGameObject
{
public:

	Spear(Math::Vector3 a_baseStartPos, float a_startDeg, float a_linePos);
	~Spear()override { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

private:

	void Init()override;
	void Release();

	//出現位置
	const Math::Vector3 SPAWNPOS = Math::Vector3(0, 0.4f, 0);

	//槍の位置
	const float SPEARPOSMAX = 1.0f;
	const float SPEARPOSSTART = -1.0f;
	const float SPEARMOVEY = 0.2f;

	float m_spearPos = SPEARPOSSTART;

	//螺旋位置
	float m_linePos;

	//角度
	float m_angleDeg;

	//槍の穴専用
	Math::Matrix m_mHoleWorld;

	//槍出現フラグ
	const float SPEARACTIVEY = 0.8f;

	bool m_isSpear = false;

	//モデル2個
	std::shared_ptr<KdModelData> m_holeModel;
	std::shared_ptr<KdModelData> m_spearModel;

};