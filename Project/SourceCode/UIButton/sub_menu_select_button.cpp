#include "sub_menu_select_button.hpp"

SubMenuSelectButton::SubMenuSelectButton(const TextData& text_data, const Vector2D<int>& center_pos, const std::function<void()> exeute_function, const bool is_init_selected) :
	UIButtonBase						(exeute_function),
	data								(SubMenuSelectButtonData()),
	m_text_data							(text_data),
	m_center_pos						(center_pos),
	m_current_alpha_blend_num			(0),
	m_destination_alpha_blend_num		(0),
	m_current_selecting_center_pos		(center_pos),
	m_destination_selecting_center_pos	(center_pos),
	m_button_frame_dark_graphic			(nullptr),
	m_button_frame_light_graphic		(nullptr),
	m_selecting_button_movie			(nullptr),
	m_mask_creator						(std::make_shared<MaskCreator>()),
	m_selecting_button_screen			(nullptr),
	m_applied_mask_screen				(nullptr),
	m_selecting_button_basic_screen		(nullptr)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/sub_menu_select_button_data.json", j_data))
	{
		data = j_data.at("sub_menu_select_button_data").get<SubMenuSelectButtonData>();
	}

	m_button_frame_dark_graphic		= std::make_shared<Graphicer>(data.dark_frame_graphic_path);
	m_button_frame_light_graphic	= std::make_shared<Graphicer>(data.light_frame_graphic_path);
	m_selecting_button_movie		= std::make_shared<MoviePlayer>(data.selecting_movie_path);
	m_selecting_button_screen		= std::make_shared<ScreenCreator>(m_button_frame_dark_graphic->GetOriginSize(), center_pos);
	m_applied_mask_screen			= std::make_shared<ScreenCreator>(m_button_frame_dark_graphic->GetOriginSize(), m_selecting_button_screen->GetHalfScreenSize());
	m_selecting_button_basic_screen = std::make_shared<ScreenCreator>(m_button_frame_dark_graphic->GetOriginSize());

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

	// 最初から選択されている場合の処理
	if (is_init_selected)
	{
		m_current_alpha_blend_num			= UCHAR_MAX;
		m_destination_selecting_center_pos	= m_center_pos + v2d::GetNormalizedV<float>(Vector2D<float>(-1.0f, -1.0f)) * data.selecting_button_distance;
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
		static_cast<int>((m_selecting_button_screen->GetScreenSize().x - m_text_data.size.x) * 0.5f),
		static_cast<int>((m_selecting_button_screen->GetScreenSize().y - m_text_data.size.y) * 0.5f),
		m_text_data.text.c_str(), m_text_data.u_int_color, m_text_data.font_handle);

	m_selecting_button_screen->UnuseScreen();
}

void SubMenuSelectButton::CalcAlphaBlendNum()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_is_active)
	{
		m_destination_alpha_blend_num = UCHAR_MAX;
		math::Increase(m_current_alpha_blend_num, static_cast<int>(data.fade_in_speed  * delta_time), m_destination_alpha_blend_num, false);
	}
	else
	{
		m_destination_alpha_blend_num = 0;
		math::Decrease(m_current_alpha_blend_num, static_cast<int>(data.fade_out_speed * delta_time),  m_destination_alpha_blend_num);
	}

	m_applied_mask_screen->GetGraphicer()->SetBlendNum(m_current_alpha_blend_num);
	m_button_frame_light_graphic		 ->SetBlendNum(m_current_alpha_blend_num);
}

void SubMenuSelectButton::CalcSelectingButtonPos()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_is_active)
	{
		m_destination_selecting_center_pos = m_center_pos + v2d::GetNormalizedV<float>(Vector2D<float>(-1.0f, -1.0f)) * data.selecting_button_distance;
	}
	else
	{
		m_destination_selecting_center_pos = m_center_pos;
	}

	auto		current_pos			= VGet(m_current_selecting_center_pos.x,     m_current_selecting_center_pos.y,     0.0f);
	const auto	destination_pos		= VGet(m_destination_selecting_center_pos.x, m_destination_selecting_center_pos.y, 0.0f);

	current_pos						= math::GetApproachedVector(current_pos, destination_pos, data.move_speed * delta_time);
	m_current_selecting_center_pos	= Vector2D<int>(current_pos.x, current_pos.y);
	m_selecting_button_screen->GetGraphicer()->SetCenterPos(m_current_selecting_center_pos);
}
