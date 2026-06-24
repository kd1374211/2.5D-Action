#include "SoundManager.h"

void SoundManager::Update()
{
	for (auto itr = m_storedSoundInstances.begin(); itr != m_storedSoundInstances.end();)
	{
		if (!itr->second.expired())
		{
			//止まっていたら削除
			if (itr->second.lock()->IsStopped())
			{
				itr = m_storedSoundInstances.erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}

	if (GetAsyncKeyState('4') & 0x8000)
	{
		if (!m_is4Key)
		{
			m_isMute = !m_isMute;
			if (m_isMute)
			{
				for (int i = 0; i < (int)SoundName::Max; i++)VolumeChange((SoundName)i, 0.0f);
			}
			else
			{
				for (int i = 0; i < (int)SoundName::Max; i++)VolumeChange((SoundName)i, 1.0f);
			}
			
		}
		m_is4Key = true;
	}
	else
	{
		m_is4Key = false;
	}
}

void SoundManager::Play(SoundName a_name)
{
	SoundData data = GetSoundData(a_name);
	if (data.m_path != "Error")
	{
		std::shared_ptr<KdSoundInstance> soundInstance = KdAudioManager::Instance().Play(data.m_path, data.m_isLoop);
		soundInstance->SetVolume(data.m_baseVolume);

		//デバッグ
		if (m_isMute)soundInstance->SetVolume(0.0f);

		//インスタンスを保存しておく
		m_storedSoundInstances.emplace(a_name, soundInstance);
	}
}

void SoundManager::Stop(SoundName a_name)
{
	auto it = m_storedSoundInstances.find(a_name);
	if (it != m_storedSoundInstances.end())
	{
		//再生中のインスタンスが存在する場合は停止する
		if (it->second.expired())return;

		if (it->second.lock()->IsPlaying())it->second.lock()->Stop();
	}
}

void SoundManager::VolumeChange(SoundName a_name, float a_volumeMulti)
{
	auto it = m_storedSoundInstances.find(a_name);
	if (it != m_storedSoundInstances.end())
	{
		//再生中のインスタンスが存在する場合は停止する
		if (it->second.expired())return;

		//音量計算
		float baseVolume = m_soundData.find(a_name)->second.m_baseVolume;
		it->second.lock()->SetVolume(baseVolume * a_volumeMulti);

		//デバッグ
		if (m_isMute)it->second.lock()->SetVolume(0.0f);
	}
}

SoundData SoundManager::GetSoundData(SoundName a_name)
{
	auto it = m_soundData.find(a_name);
	if (it != m_soundData.end())
	{
		return it->second;
	}
	else
	{
		return SoundData();
	}
}

void SoundManager::Init()
{
	LoadData();
}

void SoundManager::LoadData()
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, "Asset/Data/Sound/SoundData.csv", "r") == 0)
	{
		const int STRLENG = 100;
		char dummy[STRLENG] = {};
		int nameTag = 0;
		char path[STRLENG] = {};
		float baseVolume = 0.0f;
		int isLoop = 0;

		while (1)
		{
			if (fgets(dummy, STRLENG, fp) != nullptr)//1行読み
			{
				
				fscanf_s(fp, "%d,%[^,],%f,%d,",
					&nameTag,
					path, STRLENG,
					&baseVolume,
					&isLoop);

				m_soundData.emplace((SoundName)nameTag, SoundData{ path,baseVolume,(bool)isLoop });
			}
			else break;
		}

		fclose(fp);
	}
}

void SoundManager::Release()
{

}
