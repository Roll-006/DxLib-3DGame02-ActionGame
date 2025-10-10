#pragma once
#include <memory>

#include "../Interface/i_melee_hittable.hpp"
#include "../Part/button_graphic_getter.hpp"
#include "../Part/screen_creator.hpp"

class MeleeTargetIcon final
{
public:
	MeleeTargetIcon(std::shared_ptr<IMeleeHittable>& melee_target);
	~MeleeTargetIcon();

	void LateUpdate();
	void Draw() const;

private:
	void CalcIcconSize();
	void CreateResultScreen();

private:
	static constexpr VECTOR			kOffset				= { 0.0f, 9.0f, 0.0f };
	static constexpr float			kIconSize			= 18.0f;
	static constexpr Vector2D<int>	kScreenSize			= { 500, 500 };
	static constexpr Vector2D<int>	kScreenCenterPos	= { static_cast<int>(kScreenSize.x * 0.5f), static_cast<int>(kScreenSize.y * 0.5f) };
	static constexpr Vector2D<int>	kCursorOffset		= { 0, 100 };

	std::shared_ptr<IMeleeHittable>&		m_melee_target;
	std::shared_ptr<ButtonGraphicGetter>	m_button_graphic_resource;
	std::shared_ptr<Graphicer>				m_button_icon_graphic;
	std::shared_ptr<Graphicer>				m_cursor_graphic;
	std::shared_ptr<ScreenCreator>			m_result_screen;

	VECTOR									m_icon_pos;
	float									m_icon_size;
	bool									m_is_draw_icon;
};
