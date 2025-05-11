#include "scene_manager.hpp"

SceneObjManager::SceneObjManager()
{
	//m_scene_list[SceneKind::kTitle]		= std::make_shared<TitleScene>();
	//m_scene_list[SceneKind::kPlay]		= std::make_shared<PlayScene>();
	//m_scene_list[SceneKind::kGameClear]	= std::make_shared<GameClearScene>();
	//m_scene_list[SceneKind::kGameOver]	= std::make_shared<GameOverScene>();
	//m_scene_list[SceneKind::kLoad]		= std::make_shared<LoadScene>();
	//m_scene_list[SceneKind::kShare]		= std::make_shared<ShareScene>();

	// ‹¤—LƒV[ƒ“‚Íí‚É—¬‚·
	AttachCurrentScene(SceneKind::kShare);
}

SceneObjManager::~SceneObjManager()
{
	// ˆ—‚È‚µ
}

void SceneObjManager::Update()
{
	for (auto& scene : m_current_scene)
	{
		scene.second->Update();
	}
}

void SceneObjManager::Draw()const
{
	for (auto& scene : m_current_scene)
	{
		scene.second->Draw();
	}
}

void SceneObjManager::AttachCurrentScene(SceneKind scene_kind)
{
	m_current_scene[scene_kind] = m_scene_list.at(scene_kind);

	m_current_scene.at(scene_kind)->Init();
}

void SceneObjManager::DetachCurrentScene(SceneKind scene_kind)
{
	m_current_scene.erase(scene_kind);
}
