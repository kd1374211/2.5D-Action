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

	//開始前加速
	const float SCROLLSPEEDMAX = 4.0f;
	const float SCROLLSPEEDUP = 0.05f;
	const float SCROLLSPEEDDOWN = -0.2f;

	bool m_isScrollMaxed = false;
	float m_scrollSpeed = 1.0f;

};
