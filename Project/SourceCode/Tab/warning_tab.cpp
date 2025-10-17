#include "warning_tab.hpp"

WarningTab::WarningTab() :
	m_priority		(10),
	m_is_active		(true),
	m_back			(false),
	m_ui_selector	(std::make_shared<UISelector>(1, true, true))
{
	m_ui_selector->AddUIButton(std::make_shared<SelectButton>([this]() { ExecuteDecide(); }));
	m_ui_selector->AddUIButton(std::make_shared<SelectButton>([this]() { ExecuteBack();	}));
}

WarningTab::~WarningTab()
{

}

void WarningTab::Init()
{
	m_ui_selector->Init();

	m_back = false;
}

void WarningTab::Update()
{
	if (!m_is_active) { return; }

	m_ui_selector->Update();
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
	m_back = true;
}
