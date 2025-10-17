#include "game_over_tab.hpp"

GameOverTab::GameOverTab() : 
	m_priority		(0),
	m_is_active		(false),
	m_ui_selector	(std::make_shared<UISelector>(0, true, true))
{

}

GameOverTab::~GameOverTab()
{

}

void GameOverTab::Init()
{
	m_ui_selector->Init();
}

void GameOverTab::Update()
{
	if (!m_is_active) { return; }

	m_ui_selector->Update();
}

void GameOverTab::OnDraw() const
{
	if (!m_is_active) { return; }

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}
}
