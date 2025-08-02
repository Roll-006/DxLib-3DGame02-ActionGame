#pragma once
#include "weapon_shortcut_icon.hpp"
#include "../Object/player.hpp"

class WeaponShortcutDrawer final
{
public:
	WeaponShortcutDrawer(const std::shared_ptr<Player> player);
	~WeaponShortcutDrawer();

	void LateUpdate();
	void Draw() const;

private:
	void CreateShortcutIcon();

private:
	static constexpr Vector2D<int>	kCenterPos			= { static_cast<int>(Window::kWidth * 0.72f), Window::kHalfHeight };
	static constexpr int			kFirstIntervalPos	= 58;
	static constexpr int			kIntervalPos		= 17;
	static constexpr int			kIconWidth			= 145;
	static constexpr int			kIconHeight			= 80;

	std::shared_ptr<Player>															m_player;
	std::unordered_map<WeaponShortcutPosKind, std::shared_ptr<WeaponShortcutIcon>>	m_weapon_shortcut_icons;
	std::unordered_map<std::string, std::shared_ptr<Graphicer>>						m_weapon_graphic_pair;
	std::unordered_map<WeaponShortcutPosKind, Vector2D<int>>						m_center_pos;
};
