#include "FontsManager.h"

void FontsManager::Init()
{
	LoadFontData();
	LoadTextData();
}

void FontsManager::LoadFontData()
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, "Asset/Data/Fonts/FontsData.csv", "r") == 0)
	{
		const int STRLENG = 100;
		char dummy[STRLENG] = {};
		FontType m_index;
		char path[STRLENG] = {};
		char title[STRLENG] = {};
		int height = 0;
		
		for (int i = 0; i < FontType::FontsMax; i++)
		{
			if (fgets(dummy, STRLENG, fp) != nullptr)//1行読み
			{
				//メモ・リソースはパス、フォントはタイトル
				fscanf_s(fp, "%d,%[^,],%[^,],%d,",
					&m_index,
					path, STRLENG,
					title, STRLENG,
					&height);

				KdFontManager::Instance().AddFontResource(path);
				KdFontManager::Instance().AddFont(m_index, title, height);
			}
		}

		fclose(fp);
	}
}

void FontsManager::LoadTextData()
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, "Asset/Data/Scene/SceneTextData.csv", "r") == 0)
	{
		const int STRLENG = 100;
		char dummy[STRLENG] = {};
		char text[STRLENG] = {};
		TextScene scene;
		TextData data;

		for (int i = 0; i < Texts::Texts_Max; i++)
		{
			if (fgets(dummy, STRLENG, fp) != nullptr)//1行読み
			{
				fscanf_s(fp, "%d,%d,%d,%[^,],%d,%f,%f,",
					&data.m_Id,
					&scene,
					&data.m_type,
					text, STRLENG,
					&data.m_base,
					&data.m_pos.x,
					&data.m_pos.y);

				data.m_str = text;
				m_textData[scene].push_back(data);
			}
		}

		fclose(fp);
	}
}

void FontsManager::Release()
{}
