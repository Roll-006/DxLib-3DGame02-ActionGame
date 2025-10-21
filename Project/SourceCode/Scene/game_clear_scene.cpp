#include "game_clear_scene.hpp"
#include "../Manager/scene_manager.hpp"

GameClearScene::GameClearScene() : 
	m_is_active	(true),
	m_scene_kind(SceneKind::kGameClear)
{

}

GameClearScene::~GameClearScene()
{

}

void GameClearScene::Init()
{

}

void GameClearScene::Update()
{

}

void GameClearScene::LateUpdate()
{

}

void GameClearScene::DrawToShadowMap() const
{

}

void GameClearScene::Draw() const
{
	DrawFormatString(0, 40, 0xffffff, "GAME CLEAR");
}

std::shared_ptr<IScene> GameClearScene::ChangeScene()
{
	return nullptr;
}

void GameClearScene::StartFadeIn()
{

}
