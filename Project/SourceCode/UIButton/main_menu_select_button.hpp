#pragma once
#include "../Base/ui_button_base.hpp"
#include "../Calculation/math.hpp"
#include "../Font/font_handler.hpp"
#include "../Part/mask_creator.hpp"
#include "../Part/screen_creator.hpp"
#include "../Data/main_menu_select_button_data.hpp"
#include "../Data/text_data.hpp"

class MainMenuSelectButton final : public UIButtonBase
{
public:
	MainMenuSelectButton(const TextData& text_data, const Vector2D<int>& center_pos, const std::function<void()> exeute_function, const bool is_init_selected);
	~MainMenuSelectButton() override;

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

private:
	void CalcAlphaBlendNum();
	void CalcGraphicScale();

private:
	MainMenuSelectButtonData		data;

	TextData						m_text_data;
	int								m_alpha_blend_num;
	float							m_current_graphic_scale;
	float							m_destination_graphic_scale;
	std::shared_ptr<ScreenCreator>	m_button_screen;
};
