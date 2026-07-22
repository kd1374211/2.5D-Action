#include "HeartCharge.h"
#include "../Sound/SoundManager.h"

void HeartCharge::Reset()
{
	m_isAfterHit = false;
	m_charge = MAXCHARGE;
}

void HeartCharge::Update()
{
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

void HeartCharge::Init()
{
	Reset();
}

void HeartCharge::Release()
{}
