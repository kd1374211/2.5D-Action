#pragma once
#include "../../../../Fonts/FontsManager.h"

class ResultWindow :public KdGameObject
{
public:

	ResultWindow() { Init(); }
	~ResultWindow()override { Release(); }

	void Update()override;
	void DrawSprite()override;

	//CountF追加
	void SetCountF(int a_frame) { m_countF = a_frame; }

	//リザルト終了
	void ResultEnd() { m_isResultEnd = true; }

	//ウィンドウ撤去
	bool GetIsWindowOut()const { return m_isWindowOut; }
	
private:

	enum ResultTexts
	{
		Result,
		Heights,
		HeightsScore,
		Kills,
		KillsScore,
		Rank,
		RankText,
		Max
	};

	struct TextData
	{
		ResultTexts m_index;
		FontsManager::FontType m_font;
		std::string m_text;
		Math::Vector2 m_pos;
		KdSpriteShader::TextDrawBase m_base;
	};

	void Init()override;
	void LoadWindowData();
	void LoadTextData();
	void Release();

	Math::Vector2 m_texSize = Math::Vector2::Zero;
	Math::Vector2 m_texDrawSize = Math::Vector2::Zero;
	int m_texSplit = 0;
	int m_mapSizeX = 0;
	int m_mapSizeY = 0;
	
	//マップ配置
	std::vector<std::vector<int>> m_texMap;
	std::shared_ptr<KdTexture> m_tex = nullptr;

	//テキスト情報
	std::vector<TextData> m_texts;

	//リザルト終了
	bool m_isResultEnd = false;

	const float WINDOWMOVESPEED = 32.0f;

	//ウィンドウが画面外に出て行った
	bool m_isWindowOut = false;

	//それぞれの描画
	static const int RANDDRAWSTARTF = 20;
	static const int HEIGHTDRAWF = 50;
	static const int KILLSDRAWF = 80;
	static const int RANKTEXTRANDF = 100;
	static const int RANKTEXTDRAWF = 150;

	//リザルトカウント(ResultSceneからセットされる)
	int m_countF = 0;
};