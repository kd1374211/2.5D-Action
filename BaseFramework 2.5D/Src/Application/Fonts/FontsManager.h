#pragma once
#include "FontsManagerConst.h"

class FontsManager
{
public:

	void Init();

	//データゲット
	std::list<TextData>& GetTextData(TextScene a_scene) { return m_textData[a_scene]; }

private:

	FontsManager() {}
	~FontsManager() { Release(); }

	void LoadFontData();
	void LoadTextData();
	void Release();

	std::list<TextData> m_textData[TextScene::Max];

public:

	static FontsManager& Instance()
	{
		static FontsManager instance;
		return instance;
	}

};

#define FONTMGR FontsManager::Instance()