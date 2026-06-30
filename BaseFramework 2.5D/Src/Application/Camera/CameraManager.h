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

	//プレイヤーをさえぎっているオブジェクトの透明化
	void PreDraw(std::shared_ptr<KdCamera>& a_cameraData);

	//カメラ基礎データ
	CameraBaseData& GetCameraData(){ return m_cameraData; }
	
private:

	//プレイヤーを覆うオブジェクトの半透明化
	const float HIDEALPHA = 0.7f;

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