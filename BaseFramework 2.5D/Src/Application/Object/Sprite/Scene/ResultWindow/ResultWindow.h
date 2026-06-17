#pragma once

class ResultWindow :public KdGameObject
{
public:

	ResultWindow() { Init(); }
	~ResultWindow()override { Release(); }

	void Update()override;
	void DrawSprite()override;

	//リザルト終了
	void ResultEnd() { m_isResultEnd = true; }

	//ウィンドウ撤去
	bool GetIsWindowOut()const { return m_isWindowOut; }
	
private:

	void Init()override;
	void LoadData();
	void Release();

	Math::Vector2 m_texSize = Math::Vector2::Zero;
	Math::Vector2 m_texDrawSize = Math::Vector2::Zero;
	int m_texSplit = 0;
	int m_mapSizeX = 0;
	int m_mapSizeY = 0;
	
	//マップ配置
	std::vector<std::vector<int>> m_texMap;
	std::shared_ptr<KdTexture> m_tex = nullptr;

	//リザルト終了
	bool m_isResultEnd = false;

	const float WINDOWMOVESPEED = 32.0f;

	//ウィンドウが画面外に出て行った
	bool m_isWindowOut = false;

};