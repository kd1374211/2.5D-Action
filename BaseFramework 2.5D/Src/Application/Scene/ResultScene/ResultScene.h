#pragma once

#include"../BaseScene/BaseScene.h"

class ResultWindow;

class ResultScene : public BaseScene
{
public:

	//それぞれの描画(リザルトウィンドウから見るのでここに置く)
	static const int RANDDRAWSTARTF = 30;
	static const int HEIGHTDRAWF = 90;
	static const int KILLSDRAWF = 120;
	static const int RANKTEXTDRAWF = 180;
	static const int RESULTENDF = 210;

	ResultScene() { Init(); }
	~ResultScene() {}

private:

	void Init()  override;
	void Event() override;

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
	bool m_canResultEnd = false;

	//リザルト終了フラグ
	bool m_isResultEnd = false;

	//スペースキー
	bool m_isSpaceKey = true;

	std::weak_ptr<ResultWindow> m_wpResultWindow;

	//音フェード
	const float BGMFADESPEED = 0.005f;
	//BGM音量
	float m_resultVolumeMulti = 1.0f;

	//SE連打防止
	bool m_isRankEndSE = false;
};
