#include "HeartCharge.h"
#include "../Sound/SoundManager.h"

void HeartCharge::Reset()
{
	m_isMaxCharge = false;
	m_isHeartSpawned = false;
	m_isAfterHit = false;
	m_charge = 0.0f;
	m_targetCharge = TARGETCHARGESTART;
}

void HeartCharge::Update()
{
	//最大チェック
	if (m_charge >= m_targetCharge)
	{
		if (!m_isMaxCharge)
		{
			SOUNDMGR.Play(SoundName::SE_HeartMaxCharge);
			m_isMaxCharge = true;
		}
	}
	else
	{
		m_isMaxCharge = false;
	}

	//最大フラグが無ければチャージ減少
	if (!m_isMaxCharge)
	{
		m_charge -= m_targetCharge * CHARGEREDUCE_F;
	}

	//0を下回らない
	if (m_charge <= 0.0f)m_charge = 0.0f;
}

bool HeartCharge::CheckHeartSpawn()
{
	if (m_isMaxCharge)
	{
		//リセット＆必要量増加
		m_isMaxCharge = false;
		m_charge = 0.0f;
		m_targetCharge += TARGETCHARGEADD;
		m_isHeartSpawned = true;
		return true;
	}

	return false;
}

void HeartCharge::OnPlayerHit()
{
	//最大チャージの場合目標-被弾時減少量まで減らす
	if (m_isMaxCharge)
	{
		m_charge = m_targetCharge * (1.0f - CHARGEREDUCE_HIT);
	}
	else
	{
		m_charge -= m_targetCharge * CHARGEREDUCE_HIT;
	}

	//被弾後フラグ設定
	m_isAfterHit = true;
}

void HeartCharge::Init()
{
	Reset();
}

void HeartCharge::Release()
{}
