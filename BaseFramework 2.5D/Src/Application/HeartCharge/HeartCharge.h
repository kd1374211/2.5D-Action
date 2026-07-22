#pragma once

class HeartCharge
{
public:

	//リスタート用
	void Reset();

	void Update();

	//プレイヤー被弾時
	void OnPlayerHit();

	//落下死
	void OnPlayerFall();

	//敵撃破時
	void OnEnemyDead();
	
	//ゲッター
	float GetHealth()const { return std::clamp(m_charge / MAXCHARGE, 0.0f, 1.0f); }

	//被弾後フラグ
	bool GetIsAfterHit()const { return m_isAfterHit; }
	void SetIsAfterHit(bool a_flg) { m_isAfterHit = a_flg; }

	//落下死フラグ
	bool GetIsFallVoid()const { return m_isFallVoid; }
	void SetIsFallVoid(bool a_flg) { m_isFallVoid = a_flg; }
	
	//瀕死チェック
	bool GetIsCriticalHealth()const { return (m_charge / MAXCHARGE) <= CRITICALHEALTHBORDER ? true : false; }

private:

	HeartCharge() { Init(); }
	~HeartCharge() { Release(); }

	void Init();
	void Release();

	//瀕死ボーダー
	const float CRITICALHEALTHBORDER = 0.25f;

	//毎フレームのチャージ減少量(割合)
	const float CHARGEREDUCE_F = 0.00025f;

	//被弾時のチャージ減少量(割合)
	const float CHARGEREDUCE_HIT = 0.25f;

	//最大チャージ
	const float MAXCHARGE = 100.0f;

	//チャージ量
	float m_charge = MAXCHARGE;
	
	//被弾時減少フラグ
	bool m_isAfterHit = false;

	//落下死
	bool m_isFallVoid = false;

public:

	static HeartCharge& Instance()
	{
		static HeartCharge instance;
		return instance;
	}

};

#define HEARTCHARGE HeartCharge::Instance()