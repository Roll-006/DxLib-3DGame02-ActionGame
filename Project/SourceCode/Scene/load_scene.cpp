#include "load_scene.hpp"
#include "../Manager/scene_manager.hpp"

LoadScene::LoadScene() : 
	m_is_active	(true),
	m_scene_kind(SceneKind::kLoad)
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
	DrawFormatString(0, 40, 0xffffff, "LOAD");
}

std::shared_ptr<IScene> LoadScene::ChangeScene()
{
	return nullptr;
}
