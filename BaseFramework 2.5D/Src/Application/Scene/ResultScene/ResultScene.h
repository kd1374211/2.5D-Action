#pragma once

#include"../BaseScene/BaseScene.h"

class ResultWindow;

class ResultScene : public BaseScene
{
public:

	ResultScene() { Init(); }
	~ResultScene() {}

private:

	void Init()  override;
	void Event() override;

	Math::Vector3 m_cameraPos;
	Math::Vector3 m_cameraDeg;

	//リザルト終了フラグ
	const float TRANSITIONSCROLLADD = 1.0f;
	const float TRANSITIONSCROLLMAX = 10.0f;

	bool m_isTitleTransition = false;
	bool m_isTitleTransitionStop = false;
	bool m_isPlayerSpawned = false;
	float m_transitionScroll = 0.0f;

	float m_scrollSpeed = 0.0f;

	//時間管理
	int m_countF = 0;

	//リザルト開始フラグ
	static const int RESULTSTARTF = 20;
	bool m_isResultStart = false;

	//リザルト終了可能フラグ
	static const int RESULTENDF = 150;
	bool m_canResultEnd = false;

	//リザルト終了フラグ
	bool m_isResultEnd = false;

	//スペースキー
	bool m_isSpaceKey = true;

	std::weak_ptr<ResultWindow> m_wpResultWindow;
};
