#pragma once

#include"../BaseScene/BaseScene.h"

class TitleScene : public BaseScene
{
public :

	TitleScene() { Init(); }
	~TitleScene() {}

private :

	void Init()  override;
	void Event() override;

	Math::Vector3 m_cameraPos;
	Math::Vector3 m_cameraDeg;

};
