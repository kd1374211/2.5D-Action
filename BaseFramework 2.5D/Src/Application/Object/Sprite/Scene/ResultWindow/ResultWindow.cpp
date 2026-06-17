#include "ResultWindow.h"
#include "../../../../Const/ScreenConst.h"

#define TEXDRAWSIZEHALF (m_texDrawSize * 0.5f)

void ResultWindow::Update()
{
	if (m_isResultEnd && !m_isWindowOut)
	{
		m_pos.y -= WINDOWMOVESPEED;
		if (m_pos.y < -(SCREENSIZEHALF.y + TEXDRAWSIZEHALF.y * m_mapSizeY))m_isWindowOut = true;
	}
}

void ResultWindow::DrawSprite()
{
	//Posをもとにテクスチャマップから座標を計算して描画
	float DrawPosStartX = m_pos.x - TEXDRAWSIZEHALF.x * (m_mapSizeX - 1);
	float DrawPosStartY = m_pos.y + TEXDRAWSIZEHALF.y * (m_mapSizeY - 1);

	for (int i = 0; i < m_mapSizeY; i++)
	{
		for (int j = 0; j < m_mapSizeX; j++)
		{
			int num = m_texMap[i][j];

			Math::Vector2 drawPos = { DrawPosStartX + m_texDrawSize.x * j, DrawPosStartY - m_texDrawSize.y * i };
			Math::Rectangle rec = { (long)((num % m_texSplit) * m_texSize.x), (long)((num / m_texSplit) * m_texSize.y), (long)m_texSize.x, (long)m_texSize.y };
			KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, drawPos.x, drawPos.y, m_texDrawSize.x, m_texDrawSize.y, &rec);
		}
	}
}

void ResultWindow::Init()
{
	m_pos = Math::Vector3::Zero;
	m_tex = std::make_shared<KdTexture>();
	LoadData();
}

void ResultWindow::LoadData()
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, "Asset/Data/Scene/ResultWindowData.csv", "r") == 0)
	{
		const int STRLENG = 250;
		char dummy[STRLENG] = {};
		char path[STRLENG] = {};
		float scale = 0.0f;
		
		//最初の行スキップ
		fgets(dummy, STRLENG, fp);

		//データ読み込み
		fscanf_s(fp, "%[^,],%f,%f,%f,%d,%d,%d,",
			path, STRLENG,
			&m_texSize.x,
			&m_texSize.y,
			&scale,
			&m_texSplit,
			&m_mapSizeX,
			&m_mapSizeY);

		m_texDrawSize = m_texSize * scale;
		m_tex->Load(path);

		//行スキップ
		fgets(dummy, STRLENG, fp);

		for (int i = 0; i < m_mapSizeY; i++)
		{
			//行スキップ
			fgets(dummy, STRLENG, fp);

			std::vector<int> line;
			for (int j = 0; j < m_mapSizeX; j++)
			{
				int num = 0;
				fscanf_s(fp, "%d,", &num);
				line.push_back(num);
			}
			m_texMap.push_back(line);
		}

		fclose(fp);
	}
}

void ResultWindow::Release()
{}
