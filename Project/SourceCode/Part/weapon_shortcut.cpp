#include "weapon_shortcut.hpp"

WeaponShortcut::WeaponShortcut(const std::shared_ptr<Player> player) : 
	m_player(player)
{
	CreateShortcutIcon();

	m_weapon_graphic_pair[ObjName.ASSAULT_RIFLE] = std::make_shared<Graphicer>(UIGraphicPath.ASSAULT_RIFLE);
}

WeaponShortcut::~WeaponShortcut()
{

}

void WeaponShortcut::Draw() const
{
	const auto current_select_shortcut	= m_player->GetWeaponSelecter()->GetCurrentSelectShortcut();
	const auto current_center_pos		= m_center_pos.at(current_select_shortcut);

	for (const auto& shortcut_icon : m_weapon_shortcut_icons)
	{
		shortcut_icon.second->Draw();
	}

	DrawBox(
		static_cast<int>(current_center_pos.x - kIconWidth  * 0.5f),
		static_cast<int>(current_center_pos.y - kIconHeight * 0.5f),
		static_cast<int>(current_center_pos.x + kIconWidth  * 0.5f),
		static_cast<int>(current_center_pos.y + kIconHeight * 0.5f),
		0xffffff, FALSE);

	DrawCircle(kCenterPos.x, kCenterPos.y, 2, 0xff0000, TRUE);
	DrawCircle(current_center_pos.x, current_center_pos.y, 4, 0xff0000, TRUE);
}

void WeaponShortcut::CreateShortcutIcon()
{
	// WARNING : WeaponShortcutPosKindÇÃíËã`èáÇ…àÀë∂ÇµÇƒÇ¢ÇÈ
	for (int i = 0; i < 8; ++i)
	{
		Vector2D<int> center_pos = kCenterPos;
		const int first_offset = (0b00110011 >> i) & 1 ? -kFirstIntervalPos : kFirstIntervalPos;

		if (i % 2 == 0)
		{
			const int offset_x_size = i < 4 ? kIconWidth * 0.5f : kIconWidth * 1.5f + kIntervalPos;
			const int offset_x		= (0b00110011 >> i) & 1 ? -offset_x_size : offset_x_size;
			center_pos.x += first_offset + offset_x;
		}
		else
		{
			const int offset_y_size = i < 4 ? kIconHeight * 0.5f : kIconHeight * 1.5f + kIntervalPos;
			const int offset_y		= (0b00110011 >> i) & 1 ? -offset_y_size : offset_y_size;
			center_pos.y += first_offset + offset_y;
		}

		m_center_pos			[static_cast<WeaponShortcutPosKind>(i)] = center_pos;
		m_weapon_shortcut_icons	[static_cast<WeaponShortcutPosKind>(i)] = std::make_shared<WeaponShortcutIcon>(center_pos, kIconWidth, kIconHeight);
	}
}
