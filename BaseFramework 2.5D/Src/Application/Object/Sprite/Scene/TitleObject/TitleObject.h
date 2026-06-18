#pragma once

class TitleObject :public KdGameObject
{
public:

	TitleObject(bool a_isFadeIn);
	~TitleObject()override { Release(); }

	void Update() override;
	void DrawSprite() override;

	//フェードアウト
	void SetFadeFlg(bool a_isFade) { m_isFade = a_isFade; }
	bool GetIsFadeEnd()const { return m_isFadeEnd; }
	bool GetIsFadeInEnd()const { return !m_isFadeIn; }

private:

	//描画座標
	const Math::Vector2 TEXDRAWPOS_TITLE = Math::Vector2(-20.0f, 200.0f);
	const Math::Vector2 TEXDRAWPOS_PRESSSPACE = Math::Vector2(0.0f, -230.0f);

	void Init()override;
	void Release();

	//画像s
	std::shared_ptr<KdTexture> m_titleTex = nullptr;
	std::shared_ptr<KdTexture> m_pressSpaceTex = nullptr;

	//フェードアウト用
	const float FADESPEED = 0.025f;

	bool m_isFade = false;
	float m_alpha = 1.0f;
	bool m_isFadeEnd = false;

	//フェードイン
	bool m_isFadeIn = false;
};