#pragma once

struct SoundData
{
	std::string m_path = "Error";
	float m_volume = 0.0f;
	bool m_isLoop = false;
};

enum class SoundName
{
	BGM_Title,
	BGM_Ingame,
	BGM_Result,
	SE_Cursor,
	SE_Decision,
	SE_Run,
	SE_Jump,
	SE_Landing,
	SE_Attack,
	SE_Hit,
	SE_LowHP,
	SE_GameStart,
	Max,
};

class SoundManager
{
public:

	//再生
	void Play(SoundName a_name);

	//停止
	void Stop(SoundName a_name);

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

public:

	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	}
};

#define SOUNDMGR SoundManager::GetInstance()
