#include "title_tab.hpp"

TitleTab::TitleTab() :
	m_priority					(0),
	m_is_active					(true),
	m_can_select				(true),
	m_is_game_start				(false),
	m_is_activate_warning_tab	(false),
	m_ui_selector				(std::make_shared<UISelector>(0, true, true))
{
	std::vector<Vector2D<int>> center_pos;
	for (int i = 0; i < 3; ++i)
	{
		center_pos.emplace_back(kFirstButtonCenterPos + Vector2D<int>(0, kButtonPosInterval * i));
	}

	m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(MainMenuSelectButton::ButtonKind::kGameStart, center_pos.at(0), [this]() { ExecuteGameStart(); }, true));
	m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(MainMenuSelectButton::ButtonKind::kOption,	center_pos.at(1), [this]() { ExecuteGameStart(); }, true));
	m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(MainMenuSelectButton::ButtonKind::kExit,		center_pos.at(2), [this]() { ExecuteExit();	},		false));
}

TitleTab::~TitleTab()
{

}

void TitleTab::Init()
{
	m_ui_selector->Init();

	m_is_game_start				= false;
	m_is_activate_warning_tab	= false;
}

void TitleTab::Update()
{
	if (!m_is_active) { return; }

	if (m_can_select) { m_ui_selector->Update(); }
}

void TitleTab::OnDraw() const
{
	if (!m_is_active) { return; }

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}
}

void TitleTab::ExecuteGameStart()
{
	m_is_game_start = true;
}

void TitleTab::ExecuteExit()
{
	m_is_activate_warning_tab	= true;
	m_can_select				= false;
}
