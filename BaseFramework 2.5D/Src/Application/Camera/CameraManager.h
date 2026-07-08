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

	//スクリーンシェイク
	void SetCameraShakeFlg(bool a_flg);
	void UpdateScreenShake();
	const Math::Vector3& GetLocalCameraPos() { return m_localCameraPos; }

	//プレイヤーをさえぎっているオブジェクトの透明化
	void PreDraw(std::shared_ptr<KdCamera>& a_cameraData);

	//カメラ基礎データ
	CameraBaseData& GetCameraData(){ return m_cameraData; }
	
private:

	//プレイヤーを覆うオブジェクトの半透明化
	const float HIDEALPHA = 0.6f;

	CameraManager() { Init(); }
	~CameraManager() { Release(); }

	void Init();
	void LoadData();
	void Release();

	//データ
	CameraBaseData m_cameraData;

	//スクリーンシェイクフラグ
	static const int CAMERASHAKEF = 4;
	const Math::Vector3 CAMERASHAKEPOS[CAMERASHAKEF] = { Math::Vector3(-0.1f,-0.1f,0),Math::Vector3(0.05f,0.1f,0),Math::Vector3(-0.05f,-0.05f,0),Math::Vector3(0.1f,0.05f,0) };
	Math::Vector3 m_localCameraPos = Math::Vector3::Zero;
	bool m_isCameraShake = false;
	int m_countF_shake = 0;

public:

	static CameraManager& Instance()
	{
		static CameraManager instance;
		return instance;
	}

};

#define CAMERAMGR CameraManager::Instance()