#pragma once

class SideSpear :public KdGameObject
{
public:

	SideSpear(float a_startDeg);
	~SideSpear()override { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;

private:

	const float SPAWNPOSY = 6.21f;
	const float BASELINEPOS = 2.02f;

	void Init()override;
	void Release();

	//槍の位置
	const float SPEARPOSMAX = 3.0f;
	const float SPEARPOSSTART = -3.0f;
	const float SPEARMOVEY = 0.3f;

	float m_spearPos = SPEARPOSSTART;

	//角度
	float m_angleDeg;

	//槍の穴専用
	Math::Matrix m_mHoleWorld;

	//槍出現フラグ
	const float SPEARACTIVEY = 1.25f;

	bool m_isSpear = false;

	//モデル2個
	std::shared_ptr<KdModelData> m_holeModel;
	std::shared_ptr<KdModelData> m_spearModel;

};