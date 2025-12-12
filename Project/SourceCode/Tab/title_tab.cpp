#include "title_tab.hpp"

TitleTab::TitleTab() :
	m_tab_handle		(HandleCreator::GetInstance()->CreateHandle()),
	m_priority			(0),
	m_is_active			(true),
	m_can_select		(true),
	m_is_game_start		(false),
	m_is_exit			(false),
	m_ui_selector		(std::make_shared<UISelector>(0, true, true)),
	m_warning_exit_tab	(std::make_shared<WarningTab>(WarningTab::WarningKind::kExit)),
	m_button_prompt		(std::make_shared<ButtonPrompt>("title"))
{
	std::vector<Vector2D<int>> center_pos;
	for (int i = 0; i < 3; ++i)
	{
		center_pos.emplace_back(kFirstButtonCenterPos + Vector2D<int>(0, kButtonPosInterval * i));
	}

	m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(MainMenuSelectButton::ButtonKind::kStartGame, center_pos.at(0), [this]() { ExecuteGameStart(); }, true));
	//m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(MainMenuSelectButton::ButtonKind::kOption,	center_pos.at(1), [this]() { ExecuteOption(); },	false));
	m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(MainMenuSelectButton::ButtonKind::kExit,		center_pos.at(1), [this]() { ExecuteExit();	},		false));

	m_button_prompt->AddExplanatoryText(0, "ゲームを開始します");
	//m_button_prompt->AddExplanatoryText(1, "ゲームの各種設定を行います");
	m_button_prompt->AddExplanatoryText(1, "ゲームを終了します");

	TabDrawer::GetInstance()->AddTab(m_warning_exit_tab);
}

TitleTab::~TitleTab()
{
	TabDrawer::GetInstance()->RemoveTab(m_warning_exit_tab->GetTabHandle());
}

void TitleTab::Init()
{
	m_ui_selector->Init();

	m_is_game_start	= false;
	m_is_exit		= false;
}

void TitleTab::Update()
{
	if (!m_is_active) { return; }

	if (m_can_select) { m_ui_selector->Update(); }

	m_warning_exit_tab->Update();
	m_button_prompt->Update(m_ui_selector->GetCurrentButtonIndex());

	if (m_is_exit)
	{
		m_warning_exit_tab->Activate();
		m_can_select	= false;
		m_is_exit		= false;
	}

	if (m_warning_exit_tab->IsDecide())
	{
		const ExitGameEvent event{};
		EventSystem::GetInstance()->Publish(event);
	}
	else if (m_warning_exit_tab->IsBack())
	{
		m_warning_exit_tab->Init();
		m_can_select = true;
	}
}

void TitleTab::OnDraw(const int main_screen_handle) const
{
	if (!m_is_active) { return; }

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}

	if (!m_warning_exit_tab->IsActive()) { m_button_prompt->Draw(); }
}

void TitleTab::ExecuteGameStart()
{
	SceneFader::GetInstance()->StartFade(UCHAR_MAX, 300.0f);

	m_is_game_start = true;
	m_can_select	= false;

	EventSystem::GetInstance()->Publish(DecisionEvent());
}

void TitleTab::ExecuteOption()
{

}

void TitleTab::ExecuteExit()
{
	m_is_exit		= true;
	m_can_select	= false;

	EventSystem::GetInstance()->Publish(DecisionEvent());
}
