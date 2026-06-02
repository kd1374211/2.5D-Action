#pragma once

#include"../BaseScene/BaseScene.h"

class Player;

class GameScene : public BaseScene
{
public :

	GameScene() { Init(); }
	~GameScene() {}

	Math::Vector3 GetCameraPos()const { return m_cameraPos; }
	Math::Vector3 GetCameraAngle()const { return m_cameraDeg; }

private:

	Math::Vector3 m_cameraPos;
	Math::Vector3 m_cameraDeg;

	void Init()  override;
	void Event() override;
};
