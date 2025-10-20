#include "game_over_tab.hpp"

GameOverTab::GameOverTab() : 
	m_priority		(0),
	m_is_active		(false),
	m_can_select	(true),
	m_ui_selector	(std::make_shared<UISelector>(0, true, true))
{
	std::vector<Vector2D<int>> center_pos;
	for (int i = 0; i < 2; ++i)
	{
		center_pos.emplace_back(kFirstButtonCenterPos + Vector2D<int>(0, kButtonPosInterval * i));
	}

	m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kContinue, center_pos.at(0), [this]() { ExecuteContinue(); }, true));
	m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kQuitGame, center_pos.at(1), [this]() { ExecuteQuitGame(); }, false));
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

void GameOverTab::ExecuteContinue()
{

}

void GameOverTab::ExecuteQuitGame()
{

}
