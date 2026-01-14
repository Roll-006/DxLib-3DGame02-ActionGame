#include "warning_tab.hpp"

WarningTab::WarningTab(const TextData& text_data) :
	data					(WarningTabData()),
	m_text_data				(text_data),
	m_tab_handle			(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active				(false),
	m_can_select			(true),
	m_is_decide				(false),
	m_is_execute_back		(false),
	m_alpha_blend_num		(0),
	m_ui_selector			(std::make_shared<UISelector>(1, true, true)),
	m_warning_icon_graphic	(nullptr),
	m_result_screen			(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_button_prompt			(nullptr)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/tab_data.json", j_data))
	{
		data = j_data.at("tab_data").at("warning").get<WarningTabData>();
	}

	m_warning_icon_graphic	= std::make_shared<Graphicer>(data.warning_graphic_icon_path);
	m_button_prompt			= std::make_shared<ButtonPrompt>(data.button_prompt_name);

	// ボタンの構築
	std::vector<Vector2D<int>> center_pos;
	for (size_t i = 0; i < data.text_data.size(); ++i)
	{
		text::CreateText(data.text_data.at(i));
		center_pos.emplace_back(Window::kCenterPos + data.first_button_center_offset + Vector2D<int>(0, data.button_pos_interval * i));

		if (i == 0)
		{
			m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(data.text_data.at(i), center_pos.at(i), [this]() { ExecuteDecide(); }, false));
		}
		else if (i == 1)
		{
			m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(data.text_data.at(i), center_pos.at(i), [this]() { ExecuteBack(); }, true));
		}
	}

	// テキストの構築
	for (auto& text : data.explanatory_text_data)
	{
		text::CreateText(text);
	}

	m_warning_icon_graphic->SetCenterPos(Window::kCenterPos + data.warning_icon_offset);
	m_warning_icon_graphic->SetScale(data.warning_icon_scale);
	CalcAlphaBlendNum();
}

WarningTab::~WarningTab()
{

}

void WarningTab::Init()
{
	m_ui_selector->Init();

	m_is_active			= false;
	m_is_execute_back	= false;
}

void WarningTab::Update()
{
	if (!m_is_active) { return; }

	JudgeSelect();
	BackTab();

	if (m_can_select) { m_ui_selector->Update(); }

	m_button_prompt->Update(m_ui_selector->GetCurrentButtonIndex());
	CreateResultScreen();
	CalcAlphaBlendNum();
}

void WarningTab::OnDraw(const int main_screen_handle) const
{
	if (!m_is_active) { return; }

	m_result_screen->Draw();
}

void WarningTab::ExecuteDecide()
{
	m_is_decide = true;

	EventSystem::GetInstance()->Publish(DecisionEvent());
}

void WarningTab::ExecuteBack()
{
	m_is_execute_back = true;

	EventSystem::GetInstance()->Publish(DecisionEvent());
}

void WarningTab::JudgeSelect()
{
	const auto is_max_blend_num = m_result_screen->GetGraphicer()->GetBlendNum() >= UCHAR_MAX;
	const auto is_fading		= SceneFader::GetInstance()->IsFading();

	m_can_select = is_max_blend_num && !is_fading;
}

void WarningTab::CalcAlphaBlendNum()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (m_is_execute_back)
	{
		math::Decrease(m_alpha_blend_num, static_cast<int>(data.fade_speed * delta_time), 0);
	}
	else
	{
		math::Increase(m_alpha_blend_num, static_cast<int>(data.fade_speed * delta_time), UCHAR_MAX, false);
	}

	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}

void WarningTab::CreateResultScreen()
{
	m_result_screen->UseScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, data.alpha_blend_num);
	DrawBox(0, 0, Window::kScreenSize.x, Window::kScreenSize.y, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}

	m_warning_icon_graphic->Draw();

	DrawStringToHandle(
		static_cast<int>((m_result_screen->GetScreenSize().x - m_text_data.size.x) * 0.5f + m_text_data.offset.x),
		static_cast<int>((m_result_screen->GetScreenSize().y - m_text_data.size.y) * 0.5f + m_text_data.offset.y),
		m_text_data.text.c_str(), 
		m_text_data.u_int_color, 
		m_text_data.font_handle);

	m_button_prompt->Draw();

	m_result_screen->UnuseScreen();
}

void WarningTab::BackTab()
{
	if (m_result_screen->GetGraphicer()->GetBlendNum() < UCHAR_MAX) { return; }

	const auto command = CommandHandler::GetInstance();
	if (   command->IsExecute(CommandKind::kPause, TimeKind::kCurrent)
		|| command->IsExecute(CommandKind::kBack,  TimeKind::kCurrent))
	{
		m_is_execute_back = true;

		EventSystem::GetInstance()->Publish(BackEvent());
	}
}
