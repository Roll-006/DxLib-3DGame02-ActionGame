#pragma once
#include <memory>

#include "../Interface/i_stealth_killable.hpp"
#include "../Part/button_graphic_getter.hpp"
#include "../Part/screen_creator.hpp"
#include "../Part/mask_creator.hpp"

class StealthKillTargetIcon final
{
public:
	StealthKillTargetIcon(std::shared_ptr<IStealthKillable>& stealth_kill_target);
	~StealthKillTargetIcon();

	void LateUpdate();
	void Draw(const int main_screen_handle) const;

private:
	void CalcResultScreenCenterPos();
	void CreateStealthKillIconScreen();

private:
	static constexpr VECTOR			kIconOffset			= { 0.0f, 10.0f, 0.0f };
	static constexpr float			kIconSize			= 18.0f;
	static constexpr Vector2D<int>	kScreenSize			= { 400, 400 };
	static constexpr Vector2D<int>	kScreenCenterPos	= { static_cast<int>(kScreenSize.x * 0.5f), static_cast<int>(kScreenSize.y * 0.5f) };
	static constexpr Vector2D<int>	kGraphicOffset		= { 35, 0 };

	std::shared_ptr<IStealthKillable>&			m_stealth_kill_target;
	std::shared_ptr<ButtonGraphicGetter>		m_button_graphic_resource;
	std::shared_ptr<Graphicer>					m_button_icon_graphic;
	std::shared_ptr<Graphicer>					m_knife_graphic;
	std::shared_ptr<ScreenCreator>				m_stealth_kill_icon_screen;

	std::shared_ptr<Graphicer>				m_mask_graphic;
	std::shared_ptr<ScreenCreator>			m_mask_screen;
	std::shared_ptr<MaskCreator>			m_mask_creator;

	VECTOR									m_icon_pos;
	float									m_icon_size;
};
