#include "SceneManager.h"

#include "BaseScene/BaseScene.h"
#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"

#include "../StageSpawner/StageSpawner.h"
#include "../Const/StageConst.h"

void SceneManager::PreUpdate()
{
	// シーン切替
	if (m_currentSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}

	m_currentScene->PreUpdate();
}

void SceneManager::Update()
{
	//仮
	if (GetAsyncKeyState('1') & 0x8000)
	{
		test = false;
	}
	else if (GetAsyncKeyState('2') & 0x8000)
	{
		test = true;
	}

	m_currentScene->Update();

	testScroll += TERRAINBASEROTATY * m_scrollSpeedMulti;
	if (testScroll >= 360.0f)testScroll -= 360.0f;
	else if (testScroll < 0.0f)testScroll += 360.0f;
}

void SceneManager::PostUpdate()
{
	m_currentScene->HitCheck();
	m_currentScene->PostUpdate();

	testScroll -= m_scrollBackDeg;
	if (testScroll >= 360.0f)testScroll -= 360.0f;
	else if (testScroll < 0.0f)testScroll += 360.0f;

	KdDebugGUI::Instance().AddLog("ScrollThisFrame : %.2f\n", testScroll - testLastScroll);
	KdDebugGUI::Instance().AddLog("StageScrollDeg : %.2f\n", testScroll);
	testLastScroll = testScroll;
}

void SceneManager::PreDraw()
{
	m_currentScene->PreDraw();
}

void SceneManager::Draw()
{
	m_currentScene->Draw();
}

void SceneManager::DrawSprite()
{
	m_currentScene->DrawSprite();
}

void SceneManager::DrawDebug()
{
	m_currentScene->DrawDebug();
}

const std::list<std::shared_ptr<KdGameObject>>& SceneManager::GetObjList()
{
	return m_currentScene->GetObjList();
}

void SceneManager::AddObject(const std::shared_ptr<KdGameObject>& _obj)
{
	m_currentScene->AddObject(_obj);
}

void SceneManager::ChangeScene(SceneType _sceneType)
{
	// 次のシーンを作成し、現在のシーンにする
	switch (_sceneType)
	{
	case SceneType::Title:
		m_currentScene = std::make_shared<TitleScene>();
		break;
	case SceneType::Game:
		m_currentScene = std::make_shared<GameScene>();
		break;
	}
	
	// 現在のシーン情報を更新
	m_currentSceneType = _sceneType;
}
