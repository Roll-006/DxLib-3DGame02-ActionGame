#include "game_clear_scene.hpp"
#include "../Manager/scene_manager.hpp"

GameClearScene::GameClearScene() : 
	m_is_active	(true),
	m_scene_kind(SceneKind::kGameClear)
{
	StartFadeIn();
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

}

std::shared_ptr<IScene> GameClearScene::ChangeScene()
{
	return nullptr;
}

void GameClearScene::StartFadeIn()
{
	const auto fader = SceneFader::GetInstance();
	fader->StartFade(0, 70.0f);
}
