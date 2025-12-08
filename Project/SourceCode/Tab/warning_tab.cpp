#include "warning_tab.hpp"

WarningTab::WarningTab(const WarningKind kind) :
	m_tab_handle			(HandleCreator::GetInstance()->CreateHandle()),
	m_priority				(10),
	m_is_active				(false),
	m_can_select			(true),
	m_is_decide				(false),
	m_is_execute_back		(false),
	m_alpha_blend_num		(0),
	m_ui_selector			(std::make_shared<UISelector>(1, true, true)),
	m_warning_icon_graphic	(std::make_shared<Graphicer>(UIGraphicPath.WARNING_ICON)),
	m_result_screen			(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_font_handle			(FontHandler::GetInstance()->GetFontHandle(FontName.EXPLANATORY_TEXT)),
	m_text					(""),
	m_font_size				(v2d::GetZeroV<Vector2D<int>>())
{
	std::vector<Vector2D<int>> center_pos;
	for (int i = 0; i < 2; ++i)
	{
		center_pos.emplace_back(kFirstButtonCenterPos + Vector2D<int>(0, kButtonPosInterval * i));
	}

	m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kDecide,	center_pos.at(0), [this]() { ExecuteDecide(); },	false));
	m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kBack,	center_pos.at(1), [this]() { ExecuteBack();	},		true));

	m_warning_icon_graphic->SetCenterPos(Window::kCenterPos + Vector2D<int>(0, -200));
	m_warning_icon_graphic->SetScale(0.1f);
	CalcAlphaBlendNum();

	switch (kind)
	{
	case WarningKind::kRestart:
		m_text = "ゲームをやり直しますか？";
		break;

	case WarningKind::kQuitGame:
	case WarningKind::kExit:
		m_text = "ゲームを終了しますか？";
		break;
	}

	m_font_size = Vector2D<int>(GetDrawStringWidthToHandle(m_text.c_str(), -1, m_font_handle), GetFontSizeToHandle(m_font_handle));
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

	if (m_can_select)
	{
		m_ui_selector->Update();
	}

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

	m_can_select = is_max_blend_num && !is_fading ? true : false;
}

void WarningTab::CalcAlphaBlendNum()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (m_is_execute_back)
	{
		math::Decrease(m_alpha_blend_num, static_cast<int>(kFadeSpeed * delta_time), 0);
	}
	else
	{
		math::Increase(m_alpha_blend_num, static_cast<int>(kFadeSpeed * delta_time), UCHAR_MAX, false);
	}

	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}

void WarningTab::CreateResultScreen()
{
	m_result_screen->UseScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	DrawBox(0, 0, Window::kScreenSize.x, Window::kScreenSize.y, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}

	m_warning_icon_graphic->Draw();

	DrawStringToHandle(
		static_cast<int>((m_result_screen->GetScreenSize().x - m_font_size.x) * 0.5f),
		static_cast<int>((m_result_screen->GetScreenSize().y - m_font_size.y) * 0.5f - 100),
		m_text.c_str(), 0xffffff, m_font_handle);

	m_result_screen->UnuseScreen();
}

void WarningTab::BackTab()
{
	if (m_result_screen->GetGraphicer()->GetBlendNum() < UCHAR_MAX) { return; }

	if (CommandHandler::GetInstance()->IsExecute(CommandKind::kPause, TimeKind::kCurrent))
	{
		m_is_execute_back = true;

		EventSystem::GetInstance()->Publish(BackEvent());
	}
}
