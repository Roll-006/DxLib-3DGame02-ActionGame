#pragma once
#include "../Interface/i_grabbable.hpp"
#include "../Part/button_graphic_getter.hpp"
#include "../Part/screen_creator.hpp"
#include "../Font/font_handler.hpp"

class EscapeIcon final
{
public:
	EscapeIcon(std::shared_ptr<IGrabbable> grab_target);
	~EscapeIcon();

	void LateUpdate();
	void Draw() const;

private:
	void CalcGaugePercent();
	void CalcIconScale();
	void CalcBlurCircleParameter();
	void SetIconGraphic();

private:
	static constexpr Vector2D<int>	kScreenSize			= { 256, 256 };
	static constexpr Vector2D<int>	kScreenCenterPos	= { static_cast<int>(kScreenSize.x * 0.5f), static_cast<int>(kScreenSize.y * 0.5f) };
	static constexpr int			kGaugeRadius		= 40;
	static constexpr int			kGaugeThickness		= 11;
	static constexpr int			kBaseGaugeColor		= 0x363636;
	static constexpr int			kEscapeGaugeColor	= 0xe32d2d;
	static constexpr Vector2D<int>	kEscapeTextOffset	= { 100, 0 };
	static constexpr Vector2D<int>	kHoldTextOffset		= { 0, -90 };
	
private:
	std::shared_ptr<IGrabbable>				m_grab_target;
	InputModeKind							m_current_input_mode_kind;

	std::shared_ptr<ScreenCreator>			m_basis_circle_screen;
	std::shared_ptr<ScreenCreator>			m_escape_circle_screen;

	std::shared_ptr<ButtonGraphicGetter>	m_button_graphic_resource;
	std::shared_ptr<Graphicer>				m_button_icon_graphic;
	std::shared_ptr<Graphicer>				m_blur_circle_graphic;

	double									m_escape_gauge_percent;
	float									m_icon_scale;
	float									m_scale_sin;
	int										m_blur_circle_alpha_num;
	float									m_blur_circle_scale;
	float									m_delete_wait_timer;

	int										m_font_handle;
	std::string								m_escape_text;
	std::string								m_hold_text;
	Vector2D<int>							m_escape_font_size;
	Vector2D<int>							m_hold_font_size;
};
