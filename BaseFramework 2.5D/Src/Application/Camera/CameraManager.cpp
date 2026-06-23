#include "CameraManager.h"

void CameraManager::Update()
{
	//Reset
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		FILE* fp = nullptr;
		if (fopen_s(&fp, "Asset/Data/Test/TestCameraData.csv", "r") == 0)
		{
			fscanf_s(fp, "%f,%f,%f,",
				&m_cameraData.m_cameraPos.x,
				&m_cameraData.m_cameraPos.y,
				&m_cameraData.m_cameraPos.z
			);

			char dummy[250];
			fgets(dummy, 250, fp);

			fscanf_s(fp, "%f,%f,%f,",
				&m_cameraData.m_cameraDeg.x,
				&m_cameraData.m_cameraDeg.y,
				&m_cameraData.m_cameraDeg.z
			);

			fclose(fp);
		}
	}

	if (m_cameraData.m_cameraDeg.x > 360.0f)m_cameraData.m_cameraDeg.x -= 360.0f;
	else if (m_cameraData.m_cameraDeg.x < 0.0f)m_cameraData.m_cameraDeg.x += 360.0f;
	if (m_cameraData.m_cameraDeg.y > 360.0f)m_cameraData.m_cameraDeg.y -= 360.0f;
	else if (m_cameraData.m_cameraDeg.y < 0.0f)m_cameraData.m_cameraDeg.y += 360.0f;

	//仮セーブ
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		//一旦ストップ
		if (0)
		{
			Save();
		}
	}
}

void CameraManager::Save()
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, "Asset/Data/Test/TestCameraData.csv", "w") == 0)
	{
		fprintf_s(fp, "%f,%f,%f,\n%f,%f,%f,",
			m_cameraData.m_cameraPos.x,
			m_cameraData.m_cameraPos.y,
			m_cameraData.m_cameraPos.z,
			m_cameraData.m_cameraDeg.x,
			m_cameraData.m_cameraDeg.y,
			m_cameraData.m_cameraDeg.z
		);

		fclose(fp);
	}
}

void CameraManager::Init()
{
	LoadData();
	m_cameraData.m_viewAngle = 80;
}

void CameraManager::LoadData()
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, "Asset/Data/Test/TestCameraData.csv", "r") == 0)
	{
		fscanf_s(fp, "%f,%f,%f,",
			&m_cameraData.m_cameraPos.x,
			&m_cameraData.m_cameraPos.y,
			&m_cameraData.m_cameraPos.z
		);

		char dummy[250];
		fgets(dummy, 250, fp);

		fscanf_s(fp, "%f,%f,%f,",
			&m_cameraData.m_cameraDeg.x,
			&m_cameraData.m_cameraDeg.y,
			&m_cameraData.m_cameraDeg.z
		);

		fclose(fp);
	}
}

void CameraManager::Release()
{}
