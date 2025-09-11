#pragma once
#include "weapon_shortcut_icon.hpp"
#include "screen_creator.hpp"

#include "../Object/player.hpp"
#include "../Base/weapon_action_state_base.hpp"
#include "../Kind/player_state_kind.hpp"

class WeaponShortcutDrawer final
{
public:
	WeaponShortcutDrawer(
		std::unordered_map<TimeKind, std::shared_ptr<WeaponActionStateBase<Player>>>& state,
		const std::shared_ptr<WeaponShortcutSelecter> weapon_shortcut_selecter);

	~WeaponShortcutDrawer();

	void LateUpdate();
	void Draw() const;

private:
	void CreateShortcutIcon();
	void UpdateAnim();

private:
	static constexpr Vector2D<int>	kScreenSize			= { 1920, 1920 };
	static constexpr Vector2D<int>	kCenterPos			= { static_cast<int>(kScreenSize.x * 0.5f), static_cast<int>(kScreenSize.y * 0.5f) };
	static constexpr int			kFirstIntervalPos	= 58;
	static constexpr int			kIntervalPos		= 17;
	static constexpr int			kIconWidth			= 145;
	static constexpr int			kIconHeight			= 80;
	static constexpr float			kEnterAnimTime		= 0.1f;
	static constexpr float			kExitAnimTime		= 0.2f;
	static constexpr float			kDrawEndTime		= 2.0f;

	std::unordered_map<TimeKind, std::shared_ptr<WeaponActionStateBase<Player>>>&	m_state;
	std::shared_ptr<WeaponShortcutSelecter>											m_weapon_shortcut_selecter;

	std::unordered_map<WeaponShortcutPosKind, std::shared_ptr<WeaponShortcutIcon>>	m_weapon_shortcut_icons;
	std::unordered_map<std::string, std::shared_ptr<Graphicer>>						m_weapon_graphic_pair;
	std::unordered_map<WeaponShortcutPosKind, Vector2D<int>>						m_center_pos;
	std::shared_ptr<ScreenCreator>													m_screen_creator;

	int		m_alpha_blend_num;
	float   m_scale;
	float	m_end_draw_time;
	float	m_end_draw_timer;
	float   m_enter_scale_timer;
	float   m_exit_scale_timer;
	bool	m_is_selected;
};
