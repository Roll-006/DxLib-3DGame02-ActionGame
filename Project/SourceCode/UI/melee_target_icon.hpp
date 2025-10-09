#pragma once
#include <memory>

#include "../Interface/i_melee_hittable.hpp"
#include "../Part/button_graphic_getter.hpp"

class MeleeTargetIcon final
{
public:
	MeleeTargetIcon(std::shared_ptr<IMeleeHittable>& melee_target);
	~MeleeTargetIcon();

	void LateUpdate();
	void Draw() const;

private:
	static constexpr VECTOR kOffset = { 0.0f, 8.0f, 0.0f };

	std::shared_ptr<IMeleeHittable>&		m_melee_target;
	std::shared_ptr<ButtonGraphicGetter>	m_button_graphic;
	std::shared_ptr<Graphicer>				m_icon_graphic;

	VECTOR									m_icon_pos;
	bool									m_is_draw_icon;
};
