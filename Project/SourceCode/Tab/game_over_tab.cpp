#include "game_over_tab.hpp"

GameOverTab::GameOverTab() : 
	m_is_draw		(false),
	m_ui_selector	(std::make_shared<UISelector>(0, true, true))
{
	m_ui_selector->AddUIButton(std::make_shared<UIButton>());
	m_ui_selector->AddUIButton(std::make_shared<UIButton>());
	m_ui_selector->AddUIButton(std::make_shared<UIButton>());
	m_ui_selector->AddUIButton(std::make_shared<UIButton>());
}

GameOverTab::~GameOverTab()
{

}

void GameOverTab::Init()
{
	m_is_draw = false;

	m_ui_selector->Init();
}

void GameOverTab::Update()
{
	m_ui_selector->Update();
}

void GameOverTab::OnDraw() const
{
	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}
}
