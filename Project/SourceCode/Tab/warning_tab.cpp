#include "warning_tab.hpp"

WarningTab::WarningTab() :
	m_priority		(10),
	m_is_active		(false),
	m_can_select	(true),
	m_is_back			(false),
	m_ui_selector	(std::make_shared<UISelector>(1, true, true))
{
	std::vector<Vector2D<int>> center_pos;
	for (int i = 0; i < 3; ++i)
	{
		center_pos.emplace_back(kFirstButtonCenterPos + Vector2D<int>(0, kButtonPosInterval * i));
	}

	m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kDecide,	center_pos.at(0), [this]() { ExecuteDecide(); },	false));
	m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kBack,	center_pos.at(1), [this]() { ExecuteBack();	},		true));
}

WarningTab::~WarningTab()
{

}

void WarningTab::Init()
{
	m_ui_selector->Init();

	m_is_back = false;
}

void WarningTab::Update()
{
	if (!m_is_active) { return; }

	if (m_can_select) { m_ui_selector->Update(); }
}

void WarningTab::OnDraw() const
{
	if (!m_is_active) { return; }

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}
}

void WarningTab::ExecuteDecide()
{
	const ExitGameEvent event{};
	EventSystem::GetInstance()->Publish(event);
}

void WarningTab::ExecuteBack()
{
	m_is_back = true;
}
