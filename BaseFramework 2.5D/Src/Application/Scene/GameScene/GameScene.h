#pragma once

#include"../BaseScene/BaseScene.h"

class Player;
class Transition;

class GameScene : public BaseScene
{
public :

	GameScene() { Init(); }
	~GameScene() {}

	Math::Vector3 GetCameraPos()const { return m_cameraPos; }
	Math::Vector3 GetCameraAngle()const { return m_cameraDeg; }

private:

	std::weak_ptr<Transition> m_wpTransition;
	std::weak_ptr<Player> m_wpPlayer;
	Math::Vector3 m_cameraPos;
	Math::Vector3 m_cameraDeg;

	void Init()  override;
	void Event() override;
	
	bool m_isTransition = false;
	
};
