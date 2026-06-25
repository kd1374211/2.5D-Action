#include "SceneManager.h"

#include "BaseScene/BaseScene.h"
#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"
#include "ResultScene/ResultScene.h"

#include "../StageSpawner/StageSpawner.h"
#include "../Const/StageConst.h"
#include "../Sound/SoundManager.h"
#include "../Camera/CameraManager.h"

void SceneManager::PreUpdate()
{
	// シーン切替
	if (m_currentSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}

	//音消滅確認
	SOUNDMGR.Update();

	m_currentScene->PreUpdate();
}

void SceneManager::Update()
{
	m_currentScene->Update();

	m_currentDeg += TERRAINBASEROTATY * m_scrollSpeedMulti;
}

void SceneManager::PostUpdate()
{
	m_currentScene->HitCheck();
	m_currentScene->PostUpdate();

	m_currentDeg -= m_scrollBackDeg;
	m_diffDeg = m_currentDeg - m_lastDeg;

	if (m_currentDeg >= 360.0f)m_currentDeg -= 360.0f;
	else if (m_currentDeg < 0.0f)m_currentDeg += 360.0f;

	m_lastDeg = m_currentDeg;
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
		//最初のタイトルシーンかどうかでフェードインするかしないかを決める
		m_currentScene = std::make_shared<TitleScene>(!m_isFirstTitle);
		if (m_isFirstTitle)m_isFirstTitle = false;
		break;
	case SceneType::Game:
		m_currentScene = std::make_shared<GameScene>();
		break;
	case SceneType::Result:
		m_currentScene = std::make_shared<ResultScene>();
		break;
	}

	//新しいシーンに持ち越しオブジェクトを追加
	while (!m_sceneCarryOverObj.empty())
	{
		AddObject(m_sceneCarryOverObj.front());
		m_sceneCarryOverObj.pop_front();
	}
	
	// 現在のシーン情報を更新
	m_currentSceneType = _sceneType;
}
