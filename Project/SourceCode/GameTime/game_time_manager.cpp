#include "game_time_manager.hpp"

GameTimeManager::GameTimeManager() : 
	m_fps		(std::make_unique<FPS>()),
	m_time_scale(std::make_shared<TimeScale>())
{

}

GameTimeManager::~GameTimeManager()
{

}

void GameTimeManager::Update()
{
	m_fps->Update();
}

void GameTimeManager::Draw()
{
	m_fps->Draw();
}

void GameTimeManager::WaitTime()
{
	m_fps->Wait();
}

float GameTimeManager::GetDeltaTime(const TimeScale::LayerKind scale_layer_kind) const
{
	return m_fps->GetDeltaTime() * m_time_scale->GetTimeScale(scale_layer_kind);
}
