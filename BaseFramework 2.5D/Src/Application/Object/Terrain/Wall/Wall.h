#pragma once

class Wall :public KdGameObject
{
public:

	Wall(Math::Vector3 a_startPos);
	~Wall()override { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;

	void Respawn();

	//ゲッター
	bool GetIsDisappear()const { return m_isDisappear; }

private:

	void Init()override;
	void Release();

	std::shared_ptr<KdModelData> m_model;
	float m_angleDeg;

	//リスポーン処理関連
	bool m_isDisappear = false;
	RespawnDir m_respawnDir = RespawnDir::Up;
};