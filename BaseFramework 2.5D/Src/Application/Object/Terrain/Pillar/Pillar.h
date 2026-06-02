#pragma once

class Pillar :public KdGameObject
{
public:

	Pillar(Math::Vector3 a_startPos);
	~Pillar()override { Release(); }

	void Update()override;
	void DrawLit()override;

	void Respawn();
	
	//ゲッター
	bool GetIsDisappear()const { return m_isDisappear; }

private:

	void Init()override;
	void Release();

	Math::Vector3 m_pos;
	float m_angleDeg;
	std::shared_ptr<KdModelData> m_model;

	//リスポーン処理関連
	bool m_isDisappear = false;
	RespawnDir m_respawnDir = RespawnDir::Up;
};