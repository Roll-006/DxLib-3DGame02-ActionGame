#include "scene_manager.hpp"

SceneObjManager::SceneObjManager()
{
	m_scene_list[SceneKind::kShare]		= std::make_shared<ShareScene>();
	m_scene_list[SceneKind::kTitle]		= std::make_shared<TitleScene>();
	m_scene_list[SceneKind::kPlay]		= std::make_shared<PlayScene>();
	m_scene_list[SceneKind::kGameClear]	= std::make_shared<GameClearScene>();
	m_scene_list[SceneKind::kGameOver]	= std::make_shared<GameOverScene>();
	m_scene_list[SceneKind::kLoad]		= std::make_shared<LoadScene>();

	// 共有シーンは常に流す
	AttachCurrentScene(SceneKind::kShare);

	// TEST : 仮
	AttachCurrentScene(SceneKind::kPlay);
}

SceneObjManager::~SceneObjManager()
{
	// 処理なし
	// FIXME : この地点を抜ける際に例外が発生する
}

void SceneObjManager::Update()
{
	InputChecker::GetInstance()->Update();

	for (const auto& scene : m_current_scene)
	{
		scene.second->Update();
	}

	CollisionManager::GetInstance()->Update();
	CommandHandler	::GetInstance()->LateUpdate();
	InputChecker    ::GetInstance()->LateUpdate();
}

void SceneObjManager::Draw() const
{
	for (const auto& scene : m_current_scene)
	{
		scene.second->Draw();
	}
}

void SceneObjManager::AttachCurrentScene(const SceneKind scene_kind)
{
	m_current_scene[scene_kind] = m_scene_list.at(scene_kind);

	m_current_scene.at(scene_kind)->Init();
}

void SceneObjManager::DetachCurrentScene(const SceneKind scene_kind)
{
	m_current_scene.erase(scene_kind);
}
