#include "title_tab.hpp"

TitleTab::TitleTab() :
	m_priority					(0),
	m_is_active					(true),
	m_is_game_start				(false),
	m_is_activate_warning_tab	(false),
	m_ui_selector				(std::make_shared<UISelector>(0, true, true))
{
	m_ui_selector->AddUIButton(std::make_shared<SelectButton>([this]() { ExecuteGameStart(); }));
	m_ui_selector->AddUIButton(std::make_shared<SelectButton>([this]() { ExecuteExit();	}));
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

	m_ui_selector->Update();
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
	m_is_activate_warning_tab = true;
}
