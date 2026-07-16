#pragma once

class HeartCharge
{
public:

	//リスタート用
	void Reset();

	void Update();

	//ハート出現チェック
	bool CheckHeartSpawn();

	//プレイヤー被弾時
	void OnPlayerHit();

	//アッダー
	void AddCharge(float a_amount) { m_charge += a_amount; }

	//ゲッター
	bool GetIsMaxCharge()const { return m_isMaxCharge; }
	float GetChargeProgress()const { return std::clamp(m_charge / m_targetCharge, 0.0f, 1.0f); }

	//ハート出現後フラグ
	void SetHeartSpawned(bool a_flg) { m_isHeartSpawned = a_flg; }
	bool GetIsHeartSpawned()const { return m_isHeartSpawned; }
	
private:

	HeartCharge() { Init(); }
	~HeartCharge() { Release(); }

	void Init();
	void Release();

	//毎フレームのチャージ減少量(割合)
	const float CHARGEREDUCE_F = 0.00015f;

	//被弾時のチャージ減少量(割合)
	const float CHARGEREDUCE_HIT = 0.2f;

	//必要チャージ増加量(数値)
	const float TARGETCHARGESTART = 100.0f;
	const float TARGETCHARGEADD = 50.0f;

	//チャージ量
	float m_charge = 0.0f;
	float m_targetCharge = 0.0f;

	//最大チャージフラグ
	bool m_isMaxCharge = false;

	//ハート出現後フラグ
	bool m_isHeartSpawned = false;

public:

	static HeartCharge& Instance()
	{
		static HeartCharge instance;
		return instance;
	}

};

#define HEARTCHARGE HeartCharge::Instance()