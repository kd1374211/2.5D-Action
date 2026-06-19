#pragma once

class FontsManager
{
public:

	void Init();

	enum FontType
	{
		CP_36,
		CP_48,
		CP_72,
		FontsMax
	};

private:

	FontsManager() {}
	~FontsManager() { Release(); }

	void LoadFontData();
	void Release();

public:

	static FontsManager& Instance()
	{
		static FontsManager instance;
		return instance;
	}

};

#define FONTMGR FontsManager::Instance()