#include "title_tab.hpp"

TitleTab::TitleTab() :
	data				(TitleTabData()),
	m_tab_handle		(HandleCreator::GetInstance()->CreateHandle()),
	m_priority			(0),
	m_is_active			(true),
	m_can_select		(true),
	m_is_game_start		(false),
	m_is_exit			(false),
	m_ui_selector		(std::make_shared<UISelector>(0, true, true)),
	m_warning_exit_tab	(nullptr),
	m_button_prompt		(nullptr)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/tab_data.json", j_data))
	{
		data = j_data.at("tab_data").at("title").get<TitleTabData>();
	}

	text::CreateText(data.warning_text_data);

	m_warning_exit_tab	= std::make_shared<WarningTab>(data.warning_text_data);
	m_button_prompt		= std::make_shared<ButtonPrompt>(data.button_prompt_name);

	// ボタンプロンプト・テキストの構築
	for (size_t i = 0; i < data.explanatory_text_data.size(); ++i)
	{
		text::CreateText(data.explanatory_text_data.at(i));
		m_button_prompt->AddExplanatoryText(static_cast<int>(i), data.explanatory_text_data.at(i).text);
	}

	// UIボタン・テキストの構築
	std::vector<Vector2D<int>> center_pos;
	for (size_t i = 0; i < data.text_data.size(); ++i)
	{
		text::CreateText(data.text_data.at(i));

		center_pos.emplace_back(data.first_button_center_offset + Vector2D<int>(0, data.button_pos_interval * i));
		
		if (i == 0)
		{
			m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(data.text_data.at(i), center_pos.at(i), [this]() { ExecuteGameStart(); }, true));
		}
		else if (i == 1)
		{
			m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(data.text_data.at(i), center_pos.at(i), [this]() { ExecuteExit();	}, false));
		}
	}

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
	SceneFader::GetInstance()->StartFade(UCHAR_MAX, data.fade_speed);

	m_is_game_start = true;
	m_can_select	= false;

	EventSystem::GetInstance()->Publish(DecisionEvent());
}

void TitleTab::ExecuteOption()
{

}

void TitleTab::ExecuteExit()
{
	//m_is_exit		= true;
	//m_can_select	= false;
	
	//EventSystem::GetInstance()->Publish(DecisionEvent());
}
