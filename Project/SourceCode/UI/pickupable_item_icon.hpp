#pragma once
#include <memory>
#include <vector>

#include "../Interface/i_item.hpp"
#include "../Data/spotted_obj_data.hpp"

#include "../Part/button_graphic_getter.hpp"
#include "../Part/screen_creator.hpp"
#include "../Part/mask_creator.hpp"
#include "../Font/font_handler.hpp"
#include "../Data/pickupable_item_icon_data.hpp"

class PickupableItemIcon final
{
public:
	PickupableItemIcon(std::shared_ptr<IItem>& pickupable_item, const std::vector<SpottedObjData>& pick_up_candidate_items);
	~PickupableItemIcon();

	void LateUpdate();
	void Draw(const int main_screen_handle) const;

private:
	void CalcResultScreenCenterPos();
	void CreatePickupableItemIconScreen();
	void CreateText();

private:
	PickupableItemIconData					data;

	std::shared_ptr<IItem>&					m_pickupable_item;
	const std::vector<SpottedObjData>&		m_pick_up_candidate_items;

	std::shared_ptr<ButtonGraphicGetter>	m_button_graphic_resource;
	std::shared_ptr<Graphicer>				m_button_icon_graphic;
	std::shared_ptr<Graphicer>				m_pickupable_cursor_graphic;
	std::shared_ptr<Graphicer>				m_candidate_cursor_graphic;
	std::shared_ptr<Graphicer>				m_explanatory_text_box_blur_graphic;
	std::shared_ptr<ScreenCreator>			m_pickupable_icon_screen;

	std::shared_ptr<Graphicer>				m_mask_graphic;
	std::shared_ptr<ScreenCreator>			m_mask_screen;
	std::shared_ptr<ScreenCreator>			m_explanatory_text_screen;
	std::shared_ptr<MaskCreator>			m_mask_creator;

	Vector2D<int>							m_screen_center_pos;
	VECTOR									m_icon_pos;
	float									m_icon_size;
};
