#pragma once

class Stair :public KdGameObject
{
public:

	Stair(Math::Vector3 a_startPos, float a_startDeg, int a_stairID);
	~Stair()override { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;

	//再生成（ワープ）
	void Respawn();
	void Respawn(bool a_isStair);

	//セッター
	void SetStairFlg(bool a_flg) { m_isStairActive = a_flg; }
	void SetIsDisappear(bool a_flg) { m_isDisappear = a_flg; }
	
	//ゲッター
	bool GetStairFlg()const { return m_isStairActive; }
	bool GetIsDisappear()const { return m_isDisappear; }
	float GetAngleDeg()const { return m_angleDeg; }
	RespawnDir GetRespawnDir()const { return m_respawnDir; }
	int GetStairID()const { return m_stairID; }
	
private:

	void Init();
	void Release();

	std::shared_ptr<KdModelData> m_model;

	float m_angleDeg = 0.0f;

	//リスポーン処理関連
	bool m_isDisappear = false;
	RespawnDir m_respawnDir = RespawnDir::Up;

	//テスト
	bool m_isStairActive = true;

	//識別番号
	int m_stairID = 0;
};