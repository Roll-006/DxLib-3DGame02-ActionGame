#include "game_time_manager.hpp"

GameTimeManager::GameTimeManager() : 
	m_fps					(std::make_unique<FPS>()),
	m_time_scale_controller	(std::make_shared<TimeScaleController>())
{

}

GameTimeManager::~GameTimeManager()
{

}

void GameTimeManager::Update()
{
	m_fps->Update();
	m_time_scale_controller->Update();
}

void GameTimeManager::Draw()
{
	m_fps->Draw();
}

void GameTimeManager::WaitTime()
{
	m_fps->Wait();
}

float GameTimeManager::GetDeltaTime(const TimeScaleLayerKind scale_layer_kind) const
{
	return m_fps->GetDeltaTime() * m_time_scale_controller->GetTimeScale(scale_layer_kind);
}
