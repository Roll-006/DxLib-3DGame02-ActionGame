#pragma once
#include <memory>

#include "../Interface/i_melee_hittable.hpp"
#include "../Part/button_graphic_getter.hpp"
#include "../Part/screen_creator.hpp"
#include "../Part/mask_creator.hpp"
#include "../Font/font_handler.hpp"

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
	static constexpr VECTOR			kIconOffset			= { 0.0f, 10.0f, 0.0f };
	static constexpr float			kIconSize			= 18.0f;
	static constexpr Vector2D<int>	kScreenSize			= { 400, 400 };
	static constexpr Vector2D<int>	kScreenCenterPos	= { static_cast<int>(kScreenSize.x * 0.5f), static_cast<int>(kScreenSize.y * 0.5f) };
	static constexpr Vector2D<int>	kCursorOffset		= { 0, 60 };

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

	VECTOR									m_icon_pos;
	float									m_icon_size;

	int										m_font_handle;
	std::string								m_text;
	Vector2D<int>							m_font_size;
};
