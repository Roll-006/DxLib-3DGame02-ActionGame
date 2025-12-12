#include "sub_menu_select_button.hpp"

SubMenuSelectButton::SubMenuSelectButton(const ButtonKind button_kind, const Vector2D<int>& center_pos, const std::function<void()> exeute_function, const bool	is_init_selected) :
	UIButtonBase						(exeute_function),
	m_button_kind						(button_kind),
	m_font_handle						(-1),
	m_text								(""),
	m_center_pos						(center_pos),
	m_font_size							(v2d::GetZeroV<Vector2D<int>>()),
	m_current_alpha_blend_num			(0),
	m_destination_alpha_blend_num		(0),
	m_current_selecting_center_pos		(center_pos),
	m_destination_selecting_center_pos	(center_pos),
	m_button_frame_dark_graphic			(std::make_shared<Graphicer>(UIGraphicPath.SUB_MENU_BUTTON_FRAME_DARK)),
	m_button_frame_light_graphic		(std::make_shared<Graphicer>(UIGraphicPath.SUB_MENU_BUTTON_FRAME_LIGHT)),
	m_selecting_button_movie			(std::make_shared<MoviePlayer>(MoviePath.SUB_MENU_SELECT_BUTTON_SMOKE, MoviePlayer::BackColorKind::kBlack, true)),
	m_mask_creator						(std::make_shared<MaskCreator>()),
	m_selecting_button_screen			(std::make_shared<ScreenCreator>(m_button_frame_dark_graphic->GetOriginSize(), center_pos)),
	m_applied_mask_screen				(std::make_shared<ScreenCreator>(m_button_frame_dark_graphic->GetOriginSize(), m_selecting_button_screen->GetHalfScreenSize())),
	m_selecting_button_basic_screen		(std::make_shared<ScreenCreator>(m_button_frame_dark_graphic->GetOriginSize()))
{
	// 画像をスクリーンに変換
	m_selecting_button_basic_screen->UseScreen();
	const auto selecting_button_graphic = std::make_shared<Graphicer>(UIGraphicPath.SUB_MENU_SELECTING_BUTTON);
	selecting_button_graphic->SetCenterPos(m_selecting_button_basic_screen->GetHalfScreenSize());
	selecting_button_graphic->Draw();
	m_selecting_button_basic_screen->UnuseScreen();

	// 中心座標を設定
	m_button_frame_dark_graphic ->SetCenterPos(center_pos);
	m_button_frame_light_graphic->SetCenterPos(m_selecting_button_screen->GetHalfScreenSize());
	m_selecting_button_movie	->GetResultGraphicer()->SetCenterPos(m_selecting_button_basic_screen->GetHalfScreenSize());

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

	case ButtonKind::kRetry:
		m_font_handle = font_handler->GetFontHandle(FontName.SUB_MENU_TEXT);
		m_text = "RETRY";
		break;

	case ButtonKind::kQuitGame:
		m_font_handle = font_handler->GetFontHandle(FontName.SUB_MENU_TEXT);
		m_text = "QUIT GAME";
		break;
	}

	// フォントサイズを取得
	m_font_size = { GetDrawStringWidthToHandle(m_text.c_str(), -1, m_font_handle), GetFontSizeToHandle(m_font_handle) };

	// 最初から選択されている場合の処理
	if (is_init_selected)
	{
		m_current_alpha_blend_num			= UCHAR_MAX;
		m_destination_selecting_center_pos	= m_center_pos + v2d::GetNormalizedV<Vector2D<float>>(Vector2D<float>(-1.0f, -1.0f)) * kSelectingButtonDistance;
		m_current_selecting_center_pos		= m_destination_selecting_center_pos;
	}

	CalcAlphaBlendNum();
	CalcSelectingButtonPos();
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
	CalcSelectingButtonPos();

	m_selecting_button_movie->CreateMovieScreen();
	CreateSelectingGraphicScreen();

	Exeute();

	//printfDx("screen : %d\n", m_selecting_button_screen->GetGraphicer()->GetBlendNum());
}

void SubMenuSelectButton::Draw() const
{
	m_button_frame_dark_graphic	->Draw();
	m_selecting_button_screen	->Draw();
}

void SubMenuSelectButton::CreateSelectingGraphicScreen()
{
	// 動画にマスクを適用
	m_applied_mask_screen->UseScreen();
	m_mask_creator->CreateMask();
	m_mask_creator->UseMask(m_selecting_button_basic_screen->GetScreenHandle(), true);
	m_selecting_button_movie->Play();
	m_mask_creator->UnuseMask();
	m_mask_creator->DeleteMask();
	m_applied_mask_screen->UnuseScreen();

	// 選択した際に浮き上がる画像・テキストを一枚の画像に結合
	m_selecting_button_screen->UseScreen();

	m_applied_mask_screen		->Draw();
	m_button_frame_light_graphic->Draw();

	DrawStringToHandle(
		static_cast<int>((m_selecting_button_screen->GetScreenSize().x - m_font_size.x) * 0.5f),
		static_cast<int>((m_selecting_button_screen->GetScreenSize().y - m_font_size.y) * 0.5f),
		m_text.c_str(), 0xffffff, m_font_handle);

	m_selecting_button_screen->UnuseScreen();
}

void SubMenuSelectButton::CalcAlphaBlendNum()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_is_active)
	{
		m_destination_alpha_blend_num = UCHAR_MAX;
		math::Increase(m_current_alpha_blend_num, static_cast<int>(1000.0f * delta_time), m_destination_alpha_blend_num, false);
	}
	else
	{
		m_destination_alpha_blend_num = 0;
		math::Decrease(m_current_alpha_blend_num, static_cast<int>(600.0f * delta_time),  m_destination_alpha_blend_num);
	}

	m_applied_mask_screen->GetGraphicer()->SetBlendNum(m_current_alpha_blend_num);
	m_button_frame_light_graphic		 ->SetBlendNum(m_current_alpha_blend_num);
}

void SubMenuSelectButton::CalcSelectingButtonPos()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_is_active)
	{
		m_destination_selecting_center_pos = m_center_pos + v2d::GetNormalizedV<Vector2D<float>>(Vector2D<float>(-1.0f, -1.0f)) * kSelectingButtonDistance;
	}
	else
	{
		m_destination_selecting_center_pos = m_center_pos;
	}

	VECTOR			current_pos		= { m_current_selecting_center_pos.x, m_current_selecting_center_pos.y, 0.0f };
	const VECTOR	destination_pos = { m_destination_selecting_center_pos.x, m_destination_selecting_center_pos.y, 0.0f };

	current_pos						= math::GetApproachedVector(current_pos, destination_pos, kMoveSpeed * delta_time);
	m_current_selecting_center_pos	= Vector2D<int>(current_pos.x, current_pos.y);
	m_selecting_button_screen->GetGraphicer()->SetCenterPos(m_current_selecting_center_pos);
}
