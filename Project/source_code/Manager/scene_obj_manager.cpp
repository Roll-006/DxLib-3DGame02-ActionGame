#include "scene_obj_manager.hpp"

bool SceneObjManager::m_instantiated = false;

SceneObjManager::SceneObjManager()
{
	assert(!m_instantiated);
	m_instantiated  = true;

	m_scene_list[SceneObjName.TITLE]		= std::make_shared<TitleScene>();
	m_scene_list[SceneObjName.PLAY]			= std::make_shared<PlayScene>();
	m_scene_list[SceneObjName.GAME_CLEAR]	= std::make_shared<GameClearScene>();
	m_scene_list[SceneObjName.GAME_OVER]	= std::make_shared<GameOverScene>();
	m_scene_list[SceneObjName.SHARE]		= std::make_shared<ShareScene>();
	m_scene_list[SceneObjName.LOAD]			= std::make_shared<LoadScene>();

	AttachScene(SceneObjName.SHARE);
}

SceneObjManager::~SceneObjManager()
{
	m_instantiated = false;
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

void SceneObjManager::AttachScene(const std::string& scene_name)
{
	m_current_scene[scene_name] = m_scene_list.at(scene_name);

	m_current_scene.at(scene_name)->Init();
}

void SceneObjManager::DetachScene(const std::string& scene_name)
{
	m_current_scene.erase(scene_name);
}

std::shared_ptr<SceneObj> SceneObjManager::GetScene(const std::string & scene_name)
{
	return m_scene_list.count(scene_name) ? m_scene_list.at(scene_name) : nullptr;
}
