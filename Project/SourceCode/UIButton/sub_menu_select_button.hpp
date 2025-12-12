#pragma once
#include <memory>

#include "../Base/ui_button_base.hpp"
#include "../Calculation/math.hpp"
#include "../Font/font_handler.hpp"
#include "../Path/ui_graphic_path.hpp"
#include "../Part/movie_player.hpp"
#include "../Part/mask_creator.hpp"

class SubMenuSelectButton final : public UIButtonBase
{
public:
	enum class ButtonKind
	{
		kDecide,
		kBack,
		kContinue,
		kRetry,
		kQuitGame,
	};

public:
	SubMenuSelectButton(
		const ButtonKind			button_kind, 
		const Vector2D<int>&		center_pos, 
		const std::function<void()> exeute_function, 
		const bool					is_init_selected);

	~SubMenuSelectButton() override;

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

private:
	void CreateSelectingGraphicScreen();

	void CalcAlphaBlendNum();
	void CalcSelectingButtonPos();
	void CalcGraphicScale();

private:
	static constexpr int   kNonSelectAlphaBlendNum	= 100;
	static constexpr float kNonSelectScale			= 0.8f;
	static constexpr float kSelectScale				= 1.0f;
	static constexpr float kScaleChangeSpeed		= 5.0f;
	static constexpr float kSelectingButtonDistance = 10.0f;
	static constexpr float kMoveSpeed				= 500.0f;

	ButtonKind						m_button_kind;
	int								m_font_handle;
	std::string						m_text;
	Vector2D<int>					m_center_pos;
	Vector2D<int>					m_font_size;
	int								m_current_alpha_blend_num;
	int								m_destination_alpha_blend_num;
	Vector2D<int>					m_current_selecting_center_pos;
	Vector2D<int>					m_destination_selecting_center_pos;

	std::shared_ptr<Graphicer>		m_button_frame_dark_graphic;
	std::shared_ptr<Graphicer>		m_button_frame_light_graphic;
	std::shared_ptr<MoviePlayer>	m_selecting_button_movie;
	std::shared_ptr<MaskCreator>	m_mask_creator;

	std::shared_ptr<ScreenCreator>	m_selecting_button_screen;
	std::shared_ptr<ScreenCreator>  m_applied_mask_screen;
	std::shared_ptr<ScreenCreator>  m_selecting_button_basic_screen;
};
