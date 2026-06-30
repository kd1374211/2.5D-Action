#include "CameraManager.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/Character/CharaManager.h"
#include "../Scene/SceneManager.h"
#include "../main.h"

void CameraManager::Init()
{
	LoadData();
	m_cameraData.m_viewAngle = 80;
}

void CameraManager::PreDraw(std::shared_ptr<KdCamera>& a_cameraData)
{
	// 手順①
	// プレイヤーとカメラの座標を取得
	Math::Vector3 playerPos = CHARAMGR.GetPlayer()->GetPos();
	Math::Vector3 cameraPos = a_cameraData->GetCameraMatrix().Translation();

	// 手順②
	// 生成したRAY情報を基に実際にRAYを飛ばして当たり判定を取る
	KdCollider::RayInfo _rayInfo(KdCollider::TypeHide, cameraPos, playerPos);

	// HITした全ての結果がここに格納される！
	std::list<KdCollider::CollisionResult> _results;

	std::list<std::shared_ptr<KdGameObject>> objList = SCENEMGR.GetObjList();
	for (auto& obj : objList)
	{
		if (obj->Intersects(_rayInfo, nullptr))
		{
			obj->SetAlpha(HIDEALPHA);
		}
	}
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
