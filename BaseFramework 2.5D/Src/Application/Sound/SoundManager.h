#pragma once

struct SoundData
{
	std::string m_path = "Error";
	float m_baseVolume = 0.0f;
	bool m_isLoop = false;
};

enum class SoundName
{
	BGM_Title,
	BGM_Ingame,
	BGM_Result,
	SE_Jump,
	SE_Landing,
	SE_Attack,
	SE_Hit,
	SE_LowHP,
	SE_GameStart,
	SE_ResultRoll,
	SE_RollEnd,
	Max,
};

class SoundManager
{
public:

	void Update();

	//再生
	void Play(SoundName a_name);

	//停止
	void Stop(SoundName a_name);

	//音量変更
	void VolumeChange(SoundName a_name, float a_volumeMulti);

private:

	SoundManager() { Init(); }
	~SoundManager() { Release(); }

	void Init();
	void LoadData();
	void Release();

	//データの取得
	SoundData GetSoundData(SoundName a_name);

	std::map<SoundName, SoundData> m_soundData;
	std::map<SoundName, std::weak_ptr<KdSoundInstance>> m_storedSoundInstances;

	//デバッグ用
	bool m_isMute = false;
	bool m_is4Key = false;

public:

	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	}
};

#define SOUNDMGR SoundManager::GetInstance()
