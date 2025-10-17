#include "sub_menu_select_button.hpp"

SubMenuSelectButton::SubMenuSelectButton(const ButtonKind button_kind, const Vector2D<int>& center_pos, std::function<void()> exeute_function) :
	UIButtonBase					(exeute_function),
	m_button_kind					(button_kind),
	m_font_handle					(-1),
	m_text							(""),
	m_center_pos					(center_pos),
	m_font_size						(v2d::GetZeroV<Vector2D<int>>()),
	m_current_alpha_blend_num		(0),
	m_destination_alpha_blend_num	(0),
	m_button_frame_dark_graphic		(std::make_shared<Graphicer>(UIGraphicPath.SUB_MENU_BUTTON_FRAME_DARK)),
	m_button_frame_light_graphic	(std::make_shared<Graphicer>(UIGraphicPath.SUB_MENU_BUTTON_FRAME_LIGHT)),
	m_selecting_button_graphic		(std::make_shared<Graphicer>(UIGraphicPath.SUB_MENU_SELECTING_BUTTON)),
	m_selecting_graphic_screen		(std::make_shared<ScreenCreator>(m_button_frame_dark_graphic->GetOriginSize(), center_pos))
{
	m_button_frame_dark_graphic ->SetCenterPos(center_pos);
	m_button_frame_light_graphic->SetCenterPos(m_selecting_graphic_screen->GetHalfScreenSize());
	m_selecting_button_graphic  ->SetCenterPos(m_selecting_graphic_screen->GetHalfScreenSize());

	const auto  font_handler = FontHandler::GetInstance();

	switch (m_button_kind)
	{
	case ButtonKind::kDecide:
		m_font_handle = font_handler->GetFontHandle(FontName.EXPLANATORY_TEXT);
		m_text = "はい";
		break;

	case ButtonKind::kBack:
		m_font_handle = font_handler->GetFontHandle(FontName.EXPLANATORY_TEXT);
		m_text = "いいえ";
		break;

	case ButtonKind::kContinue:
		m_font_handle = font_handler->GetFontHandle(FontName.SUB_MENU_TEXT);
		m_text = "CONTINUE";
		break;

	case ButtonKind::kQuitGame:
		m_font_handle = font_handler->GetFontHandle(FontName.SUB_MENU_TEXT);
		m_text = "QUIT GAME";
		break;
	}

	m_font_size.x = GetDrawStringWidthToHandle(m_text.c_str(), -1, m_font_handle);
	m_font_size.y = GetFontSizeToHandle(m_font_handle);

	CreateSelectingGraphicScreen();
}

SubMenuSelectButton::~SubMenuSelectButton()
{

}

void SubMenuSelectButton::Init()
{

}

void SubMenuSelectButton::Update()
{
	CalcAlphaBlendNum();
	CreateSelectingGraphicScreen();

	Exeute();
}

void SubMenuSelectButton::Draw() const
{
	m_button_frame_dark_graphic->Draw();
	m_selecting_graphic_screen ->Draw();
}

void SubMenuSelectButton::CreateSelectingGraphicScreen()
{
	// 選択した際に浮き上がる画像・テキストを一枚の画像に結合
	m_selecting_graphic_screen->UseScreen();

	m_selecting_button_graphic	->Draw();
	m_button_frame_light_graphic->Draw();
	DrawStringToHandle(
		static_cast<int>((m_selecting_graphic_screen->GetScreenSize().x - m_font_size.x) * 0.5f),
		static_cast<int>((m_selecting_graphic_screen->GetScreenSize().y - m_font_size.y) * 0.5f),
		m_text.c_str(), 0xffffff, m_font_handle);

	m_selecting_graphic_screen->UnuseScreen();
}

void SubMenuSelectButton::CalcAlphaBlendNum()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_is_active)
	{
		m_destination_alpha_blend_num = 255;
		math::Increase(m_current_alpha_blend_num, 500.0f * delta_time, m_destination_alpha_blend_num, false);
	}
	else
	{
		m_destination_alpha_blend_num = 0;
		math::Decrease(m_current_alpha_blend_num, 300.0f * delta_time, m_destination_alpha_blend_num);
	}

	m_selecting_button_graphic->SetAlphaBlendNum(m_current_alpha_blend_num);
}
