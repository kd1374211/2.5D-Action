#pragma once

class FontsManager
{
public:

	void Init();

	enum FontType
	{
		CP_72,
		CP_48,
		CP_36,
		CP_24,
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