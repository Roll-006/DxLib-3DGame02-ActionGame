#include "scene_manager.hpp"

SceneManager::SceneManager() : 
	m_shadow_map(std::make_unique<ShadowMap>())
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

SceneManager::~SceneManager()
{
	// 処理なし
	// FIXME : この地点を抜ける際に例外が発生する
}

void SceneManager::Update()
{
	InputChecker  ::GetInstance()->Update();
	CommandHandler::GetInstance()->Update();
	PhysicsManager::GetInstance()->Update();

	for (const auto& scene : m_current_scene)
	{
		scene.second->Update();
	}
}

void SceneManager::LateUpdate()
{
	PhysicsManager::GetInstance()->LateUpdate();

	for (const auto& scene : m_current_scene)
	{
		scene.second->LateUpdate();
	}

	CollisionManager::GetInstance()->LateUpdate();
	CommandHandler	::GetInstance()->LateUpdate();
	InputChecker	::GetInstance()->LateUpdate();
}

void SceneManager::Draw() const
{
	m_shadow_map->PrepareDrawShadowMap();
	for (const auto& scene : m_current_scene)
	{
		scene.second->DrawToShadowMap();
	}
	m_shadow_map->EndDrawShadowMap();

	m_shadow_map->UseShadowMap();
	for (const auto& scene : m_current_scene)
	{
		scene.second->Draw();
	}
	m_shadow_map->UnuseShadowMap();

	DrawEffekseer3D();
	UIDrawer::GetInstance()->Draw();
}

std::vector<SceneKind> SceneManager::GetCurrentSceneKind() const
{
	std::vector<SceneKind> current_scene_kind;

	for (const auto& scene : m_current_scene)
	{
		current_scene_kind.emplace_back(scene.first);
	}
	return current_scene_kind;
}

void SceneManager::AttachCurrentScene(const SceneKind scene_kind)
{
	m_current_scene[scene_kind] = m_scene_list.at(scene_kind);

	m_current_scene.at(scene_kind)->Init();
}

void SceneManager::DetachCurrentScene(const SceneKind scene_kind)
{
	m_current_scene.erase(scene_kind);
}
