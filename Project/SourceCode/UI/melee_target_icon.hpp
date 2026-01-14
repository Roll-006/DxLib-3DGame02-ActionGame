#pragma once
#include <memory>

#include "../Interface/i_melee_hittable.hpp"
#include "../Part/button_graphic_getter.hpp"
#include "../Part/screen_creator.hpp"
#include "../Part/mask_creator.hpp"
#include "../Font/font_handler.hpp"
#include "../Data/melee_target_icon_data.hpp"

class MeleeTargetIcon final
{
public:
	MeleeTargetIcon(std::shared_ptr<IMeleeHittable>& melee_target, std::shared_ptr<IMeleeHittable>& visible_downed_character);
	~MeleeTargetIcon();

	void LateUpdate();
	void Draw(const int main_screen_handle) const;

private:
	void CalcResultScreenCenterPos();
	void CreateMeleeIconScreen();
	void CreateDownIconScreen();

private:
	MeleeTargetIconData						data;

	std::shared_ptr<IMeleeHittable>&		m_melee_target;
	std::shared_ptr<IMeleeHittable>&		m_visible_downed_character;
	std::shared_ptr<ButtonGraphicGetter>	m_button_graphic_resource;
	std::shared_ptr<Graphicer>				m_button_icon_graphic;
	std::shared_ptr<Graphicer>				m_melee_cursor_graphic;
	std::shared_ptr<Graphicer>				m_down_cursor_graphic;
	std::shared_ptr<Graphicer>				m_explanatory_text_box_blur_graphic;
	std::shared_ptr<ScreenCreator>			m_melee_icon_screen;

	std::shared_ptr<Graphicer>				m_mask_graphic;
	std::shared_ptr<ScreenCreator>			m_mask_screen;
	std::shared_ptr<ScreenCreator>			m_explanatory_text_screen;
	std::shared_ptr<MaskCreator>			m_mask_creator;

	Vector2D<int>							m_screen_center_pos;
	VECTOR									m_icon_pos;
	float									m_icon_size;
};
