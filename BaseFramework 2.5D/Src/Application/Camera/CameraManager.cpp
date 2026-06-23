#include "CameraManager.h"

void CameraManager::Update()
{
	if (GetAsyncKeyState('H') & 0x8000)
	{
		m_cameraData.m_cameraPos.x += 0.1f;
	}
	if (GetAsyncKeyState('F') & 0x8000)
	{
		m_cameraData.m_cameraPos.x -= 0.1f;
	}
	if (GetAsyncKeyState('Y') & 0x8000)
	{
		m_cameraData.m_cameraPos.y += 0.1f;
	}
	if (GetAsyncKeyState('V') & 0x8000)
	{
		m_cameraData.m_cameraPos.y -= 0.1f;
	}
	if (GetAsyncKeyState('T') & 0x8000)
	{
		m_cameraData.m_cameraPos.z += 0.1f;
	}
	if (GetAsyncKeyState('B') & 0x8000)
	{
		m_cameraData.m_cameraPos.z -= 0.1f;
	}

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

	KdDebugGUI::Instance().AddLog("CameraMoveCenter : G\n");
	KdDebugGUI::Instance().AddLog("CameraPos : %.1f,%.1f,%.1f\n", m_cameraData.m_cameraPos.x, m_cameraData.m_cameraPos.y, m_cameraData.m_cameraPos.z);
	KdDebugGUI::Instance().AddLog("CameraDeg : %.1f,%.1f,%.1f\n", m_cameraData.m_cameraDeg.x, m_cameraData.m_cameraDeg.y, m_cameraData.m_cameraDeg.z);

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
