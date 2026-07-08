#pragma once

class ResultWindow;

class ResultStars :public KdGameObject
{
public:

	ResultStars() { Init(); }
	~ResultStars() { Release(); }

	void Update()override;
	void DrawSprite()override;

	void SetOwner(const std::shared_ptr<ResultWindow>& a_owner) { m_wpOwner = a_owner; }

private:

	const Math::Vector2 TEXBASESIZE = Math::Vector2(25.0f, 32.0f);

	enum class StarIndex
	{
		Heights,
		Kills,
		Total,
		Number
	};
	const int STARNUM[(int)StarIndex::Number] = { 5,5,10 };
	const Math::Vector2 STARDRAWSTARTPOS[(int)StarIndex::Number] = { Math::Vector2(80.0f,45.0f),Math::Vector2(80.0f,-45.0f),Math::Vector2(-112.5f,-185.0f) };

	enum StarColorType
	{
		Height,
		Kill,
		Gold,
		Empty
	};

	void Init()override;
	void Release() {}

	std::shared_ptr<KdTexture> m_tex = nullptr;

	int m_killRank = 0;
	int m_heightRank = 0;

	std::weak_ptr<ResultWindow> m_wpOwner;

};