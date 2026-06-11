#pragma once

class BaseScene;

class SceneManager
{
public :

	// シーン情報
	enum class SceneType
	{
		Title,
		Game,
	};

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();
	void DrawDebug();

	// 次のシーンをセット (次のフレームから切り替わる)
	void SetNextScene(SceneType _nextScene)
	{
		m_nextSceneType = _nextScene;
	}

	// シーン種取得
	SceneType GetSceneType()const { return m_currentSceneType; }

	// 現在のシーンのオブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList();

	// 現在のシーンにオブジェクトを追加
	void AddObject(const std::shared_ptr<KdGameObject>& _obj);

	//現在のシーンにスクロール量を設定
	void SetScrollSpeedMulti(float a_speed) {
		if (!test)m_scrollSpeedMulti = 0.0f;
		else m_scrollSpeedMulti = a_speed; }

	//スクロール速度を取得
	float GetScrollSpeedMulti()const { return m_scrollSpeedMulti; }

	//スクロールバック
	void SetScrollBack(float a_deg) { m_scrollBackDeg = a_deg; }
	float GetScrollBack()const { return m_scrollBackDeg; }

private :

	// マネージャーの初期化
	// インスタンス生成(アプリ起動)時にコンストラクタで自動実行
	void Init()
	{
		// 開始シーンに切り替え
		ChangeScene(m_currentSceneType);
	}

	// シーン切り替え関数
	void ChangeScene(SceneType _sceneType);

	// 現在のシーンのインスタンスを保持しているポインタ
	std::shared_ptr<BaseScene> m_currentScene = nullptr;

	// 現在のシーンの種類を保持している変数
	SceneType m_currentSceneType = SceneType::Title;
	
	// 次のシーンの種類を保持している変数
	SceneType m_nextSceneType = m_currentSceneType;

	//スクロール速度保持
	float m_scrollSpeedMulti = 0.0f;
	float m_scrollBackDeg = 0.0f;

	float testLastScroll = 0.0f;
	float testScroll = 0.0f;

	//テスト
	bool test = true;
	
private:

	SceneManager() { Init(); }
	~SceneManager() {}

public:

	// シングルトンパターン
	// 常に存在する && 必ず1つしか存在しない(1つしか存在出来ない)
	// どこからでもアクセスが可能で便利だが
	// 何でもかんでもシングルトンという思考はNG
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}
};

#define SCENEMGR SceneManager::Instance()