#pragma once
#include "../Interface/i_grabbable.hpp"
#include "../Part/button_graphic_getter.hpp"
#include "../Part/screen_creator.hpp"
#include "../Font/font_handler.hpp"
#include "../Data/escape_icon_data.hpp"

class EscapeIcon final
{
public:
	EscapeIcon(const std::shared_ptr<IGrabbable>& grab_target);
	~EscapeIcon();

	void LateUpdate();
	void Draw() const;

private:
	void CalcGaugePercent();
	void CalcIconScale();
	void CalcBlurCircleParameter();
	void SetIconGraphic();
	
private:
	EscapeIconData							data;

	const std::shared_ptr<IGrabbable>		m_grab_target;
	InputModeKind							m_current_input_mode_kind;

	std::shared_ptr<ScreenCreator>			m_basis_circle_screen;
	std::shared_ptr<ScreenCreator>			m_escape_circle_screen;

	std::shared_ptr<ButtonGraphicGetter>	m_button_graphic_resource;
	std::shared_ptr<Graphicer>				m_button_icon_graphic;
	std::shared_ptr<Graphicer>				m_blur_circle_graphic;

	Vector2D<int>							m_screen_center_pos;
	double									m_escape_gauge_percent;
	float									m_icon_scale;
	float									m_scale_sin;
	int										m_blur_circle_alpha_num;
	float									m_blur_circle_scale;
	float									m_delete_wait_timer;
};
