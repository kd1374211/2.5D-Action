#pragma once

class Stair :public KdGameObject
{
public:

	Stair(Math::Vector3 a_startPos, float a_startDeg);
	~Stair()override { Release(); }

	void Update();
	void DrawLit();

	//再生成（ワープ）
	void Respawn(bool a_test);

	//ゲッター
	bool GetIsDisappear()const { return m_isDisappear; }
	float GetAngleDeg()const { return m_angleDeg; }
	RespawnDir GetRespawnDir()const { return m_respawnDir; }
	
private:

	void Init();
	void Release();

	std::shared_ptr<KdModelData> m_model;

	Math::Vector3 m_pos;
	float m_angleDeg;

	//リスポーン処理関連
	bool m_isDisappear = false;
	RespawnDir m_respawnDir = RespawnDir::Up;

	//テスト
	bool test = false;
};