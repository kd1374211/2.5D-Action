#pragma once

//カメラデータ
struct CameraBaseData
{
	int m_viewAngle;
	Math::Vector3 m_cameraPos;
	Math::Vector3 m_cameraDeg;
};

class CameraManager
{
public:

	void Update();

	//カメラ基礎データ
	CameraBaseData& GetCameraData(){ return m_cameraData; }
	
	//セーブ
	void Save();

private:

	CameraManager() { Init(); }
	~CameraManager() { Release(); }

	void Init();
	void LoadData();
	void Release();

	//データ
	CameraBaseData m_cameraData;

public:

	static CameraManager& Instance()
	{
		static CameraManager instance;
		return instance;
	}

};

#define CAMERAMGR CameraManager::Instance()