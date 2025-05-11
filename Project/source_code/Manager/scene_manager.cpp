#include "scene_manager.hpp"

bool SceneObjManager::m_instantiated = false;

SceneObjManager::SceneObjManager()
{
	m_scene_list[SceneKind::kTitle]		= std::make_shared<TitleScene>();
	m_scene_list[SceneKind::kPlay]		= std::make_shared<PlayScene>();
	m_scene_list[SceneKind::kGameClear]	= std::make_shared<GameClearScene>();
	m_scene_list[SceneKind::kGameOver]	= std::make_shared<GameOverScene>();
	m_scene_list[SceneKind::kLoad]		= std::make_shared<LoadScene>();
	m_scene_list[SceneKind::kShare]		= std::make_shared<ShareScene>();

	// ‹¤—LƒV[ƒ“‚Íí‚É—¬‚·
	AttachCurrentScene(SceneKind::kShare);
}

SceneObjManager::~SceneObjManager()
{
	// ˆ—‚È‚µ
}

void SceneObjManager::Init()
{
	// –¢ŽÀ‘•
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

std::shared_ptr<SceneObj> SceneObjManager::GetScene(const std::string & scene_name)
{
	return m_scene_list.count(scene_name) ? m_scene_list.at(scene_name) : nullptr;
}
