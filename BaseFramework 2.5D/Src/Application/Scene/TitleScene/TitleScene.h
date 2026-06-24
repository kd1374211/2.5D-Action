#pragma once

#include"../BaseScene/BaseScene.h"

class TitleObject;

class TitleScene : public BaseScene
{
public :

	TitleScene(bool a_isFadeIn);
	~TitleScene() {}

private :

	void Init()  override;
	void Event() override;

	Math::Vector3 m_cameraPos;
	Math::Vector3 m_cameraDeg;

	//スペースキー
	bool m_isSpaceKey = true;

	std::weak_ptr<TitleObject> m_wpTitleObject;

	//フェードアウト
	bool m_isFadeStart = false;

	//フェードインフラグ
	bool m_isFadeIn = false;

	//音量フェード
	const float BGMFADESPEED = 0.05f;

	//BGM音量
	float m_titleVolumeMulti = 1.0f;

	//スタート後のスクロール加速
	float m_scrollSpeed = 1.0f;
	const float SCROLLADD_AFTERSTART = 0.025f;
};
