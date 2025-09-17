#include "game_over_scene.hpp"
#include "../Manager/scene_manager.hpp"

GameOverScene::GameOverScene() : 
	m_is_active(true)
{

}

GameOverScene::~GameOverScene()
{

}

void GameOverScene::Init()
{

}

void GameOverScene::Update()
{

}

void GameOverScene::LateUpdate()
{

}

void GameOverScene::DrawToShadowMap() const
{

}

void GameOverScene::Draw() const
{
	DrawFormatString(0, 40, 0xffffff, "GAME OVER");
}

std::shared_ptr<IScene> GameOverScene::ChangeScene()
{
	return nullptr;
}
