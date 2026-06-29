#include "CameraManager.h"

void CameraManager::Init()
{
	LoadData();
	m_cameraData.m_viewAngle = 80;
}

void CameraManager::PreDraw()
{}

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
