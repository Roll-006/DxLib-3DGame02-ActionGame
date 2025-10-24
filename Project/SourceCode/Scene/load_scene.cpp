#include "load_scene.hpp"
#include "../Manager/scene_manager.hpp"

LoadScene::LoadScene(const SceneKind next_scene_kind) :
	m_is_active			(true),
	m_scene_kind		(SceneKind::kLoad),
	m_next_scene_kind	(next_scene_kind)
{

}

LoadScene::~LoadScene()
{

}

void LoadScene::Init()
{

}

void LoadScene::Update()
{

}

void LoadScene::LateUpdate()
{

}

void LoadScene::DrawToShadowMap() const
{

}

void LoadScene::Draw() const
{
	
}

std::shared_ptr<IScene> LoadScene::ChangeScene()
{
	// ƒvƒŒƒC
	if (m_next_scene_kind == SceneKind::kPlay)
	{
		return std::make_shared<PlayScene>();
	}

	return nullptr;
}

void LoadScene::StartFadeIn()
{

}
