#pragma once

#include"../BaseScene/BaseScene.h"

class GameScene : public BaseScene
{
public :

	GameScene()  { Init(); }
	~GameScene() {}

private:

	int m_countF;
	float m_cameraDeg;

	void Event() override;
	void Init()  override;
};
