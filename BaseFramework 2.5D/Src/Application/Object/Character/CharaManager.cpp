#include "CharaManager.h"
#include "../../Scene/BaseScene/BaseScene.h"
#include "../Character/Player/Player.h"

void CharaManager::StartGame(BaseScene* a_scene)
{
	a_scene->AddObject(m_player);
}

void CharaManager::Update()
{
}

void CharaManager::Init()
{
	LoadPlayerTexData();
	MakeShared();
}

void CharaManager::LoadPlayerTexData()
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, "Asset/Data/Chara/PlayerTexData.csv", "r") == 0)
	{
		const int STRLENG = 100;
		Math::Vector2 rect = Math::Vector2::Zero;
		float scaleDiv = 1.0f;
		char dummy[STRLENG] = {};

		fgets(dummy, STRLENG, fp);
		fscanf_s(fp, "%f,%f,%f", &rect.x, &rect.y, &scaleDiv);
		fgets(dummy, STRLENG, fp);

		for (int i = 0; i < PlayerAnimType::Number; i++)
		{
			if (fgets(dummy, STRLENG, fp) != nullptr)//1行読み
			{
				char path[STRLENG] = {};
				float animMax = 0.0f;
				float animAdd = 0.0f;

				fscanf_s(fp, "%[^,],%[^,],%f,%f,",
					dummy, STRLENG,
					path, STRLENG,
					&animMax,
					&animAdd);

				std::shared_ptr<KdSquarePolygon> polygon = std::make_shared<KdSquarePolygon>();
				polygon->SetMaterial(path);
				polygon->SetSplit((int)animMax, 1);
				polygon->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
				polygon->SetScale(rect / scaleDiv);
				PolygonData data = { animMax,animAdd,polygon };

				m_playerPolygonData.emplace((PlayerAnimType)i, data);
			}
		}

		fclose(fp);
	}
}

void CharaManager::MakeShared()
{
	m_player = std::make_shared<Player>();
	m_player->SetPolygonData(&m_playerPolygonData);
}

void CharaManager::Release()
{}
