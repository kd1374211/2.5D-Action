#include "FontsManager.h"

void FontsManager::Init()
{
	LoadFontData();
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

void FontsManager::Release()
{}
