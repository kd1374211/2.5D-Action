#pragma once

#include"../BaseScene/BaseScene.h"

class Player;
class Transition;

class GameScene : public BaseScene
{
public :

	GameScene() { Init(); }
	~GameScene() {}
	void PreDraw()override;

private:

	std::weak_ptr<Transition> m_wpTransition;
	std::weak_ptr<Player> m_wpPlayer;

	void Init()  override;
	void Event() override;
	
	bool m_isTransition = false;

	//ゲーム開始カウンター
	static const int FADEINF = 10;
	static const int STARTF = 30;
	int m_countF_start = 0;

	//フラグたち
	bool m_isFadeIn = false;
	bool m_isGameStart = false;
};
