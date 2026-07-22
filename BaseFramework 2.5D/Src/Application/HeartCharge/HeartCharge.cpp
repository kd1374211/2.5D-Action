#include "HeartCharge.h"
#include "../Sound/SoundManager.h"
#include "../Score/ScoreManager.h"
#include "../StageSpawner/StageSpawner.h"

void HeartCharge::Reset()
{
	m_isAfterHit = false;
	m_isFallVoid = false;
	m_charge = MAXCHARGE;
}

void HeartCharge::Update()
{
	//最大越えのチャージをカット
	if (m_charge > MAXCHARGE)m_charge = MAXCHARGE;

	//チャージ減少
	m_charge -= MAXCHARGE * CHARGEREDUCE_F;

	//0を下回らない
	if (m_charge <= 0.0f)m_charge = 0.0f;
}

void HeartCharge::OnPlayerHit()
{
	//ゲージ減少
	m_charge -= MAXCHARGE * CHARGEREDUCE_HIT;

	//被弾後フラグ設定
	m_isAfterHit = true;
}

void HeartCharge::OnPlayerFall()
{
	m_charge = 0.0f;

	//落下死フラグ
	m_isFallVoid = true;
}

void HeartCharge::OnEnemyDead()
{
	//瀕死の時は回復量増加
	m_charge += STAGESPAWNER.GetHealPower() * (GetIsCriticalHealth() ? 1.25f : 1.0f);
}

void HeartCharge::Init()
{
	Reset();
}

void HeartCharge::Release()
{}
