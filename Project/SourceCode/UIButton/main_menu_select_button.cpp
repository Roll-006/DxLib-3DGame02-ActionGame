#include "main_menu_select_button.hpp"

MainMenuSelectButton::MainMenuSelectButton(const ButtonKind button_kind, const Vector2D<int>& center_pos, const std::function<void()> exeute_function, const bool is_init_selected) :
	UIButtonBase				(exeute_function),
	m_button_kind				(button_kind),
	m_alpha_blend_num			(kNonSelectAlphaBlendNum),
	m_current_graphic_scale		(kNonSelectScale),
	m_destination_graphic_scale	(kNonSelectScale),
	m_button_screen				(nullptr)
{
	const auto  font_handle		= FontHandler::GetInstance()->GetFontHandle(FontName.MAIN_MENU_TEXT);
	const auto  font_height		= GetFontSizeToHandle(font_handle);
	const auto  screen_height	= font_height + kScreenHeightOffset;
	std::string text			= "";

	switch (m_button_kind)
	{
	case ButtonKind::kStartGame:
		text = "START GAME";
		break;

	case ButtonKind::kReturnToGame:
		text = "RETURN TO GAME";
		break;

	case ButtonKind::kRestart:
		text = "RESTART";
		break;

	case ButtonKind::kOption:
		text = "OPTION";
		break;

	case ButtonKind::kQuitGame:
		text = "QUIT GAME";
		break;

	case ButtonKind::kExit:
		text = "EXIT";
		break;
	}

	m_button_screen = std::make_shared<ScreenCreator>(Vector2D<int>(kScreenWidth, screen_height), center_pos);
	m_button_screen->UseScreen();
	DrawStringToHandle(
		kScreenHeightOffset,
		static_cast<int>((screen_height - font_height) * 0.5f),
		text.c_str(), 0xffffff, font_handle);
	m_button_screen->UnuseScreen();

	// Å‰‚©‚ç‘I‘ð‚³‚ê‚Ä‚¢‚éê‡‚Ìˆ—
	if (is_init_selected)
	{
		m_alpha_blend_num		= UCHAR_MAX;
		m_current_graphic_scale = kSelectScale;
	}

	CalcAlphaBlendNum();
	CalcGraphicScale();
}

MainMenuSelectButton::~MainMenuSelectButton()
{

}

void MainMenuSelectButton::Init()
{

}

void MainMenuSelectButton::Update()
{
	CalcAlphaBlendNum();
	CalcGraphicScale();

	Exeute();
}

void MainMenuSelectButton::Draw() const
{
	m_button_screen->Draw();
}

void MainMenuSelectButton::CalcAlphaBlendNum()
{
	m_alpha_blend_num = m_is_active ? UCHAR_MAX : kNonSelectAlphaBlendNum;
	m_button_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}

void MainMenuSelectButton::CalcGraphicScale()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_is_active)
	{
		m_destination_graphic_scale = kSelectScale;
		math::Increase(m_current_graphic_scale, kScaleChangeSpeed * delta_time, m_destination_graphic_scale, false);
	}
	else
	{
		m_destination_graphic_scale = kNonSelectScale;
		math::Decrease(m_current_graphic_scale, 3.0f * delta_time, m_destination_graphic_scale);
	}

	m_button_screen->GetGraphicer()->SetScale(m_current_graphic_scale);
}
