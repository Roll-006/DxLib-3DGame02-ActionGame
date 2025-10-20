#include "weapon_shortcut_icon.hpp"

WeaponShortcutIcon::WeaponShortcutIcon(const Vector2D<int>& center_pos, const int width, const int height) :
	m_center_pos	(center_pos),
	m_icon_width	(width),
	m_icon_height	(height),
	m_graphicer		(nullptr)
{
	// èàóùÇ»Çµ
}

WeaponShortcutIcon::~WeaponShortcutIcon()
{

}

void WeaponShortcutIcon::Draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlphaBlendNum);

	DrawBox(
		static_cast<int>(m_center_pos.x - m_icon_width  * 0.5f),
		static_cast<int>(m_center_pos.y - m_icon_height * 0.5f),
		static_cast<int>(m_center_pos.x + m_icon_width  * 0.5f),
		static_cast<int>(m_center_pos.y + m_icon_height * 0.5f),
		0x000000, TRUE);

	DrawBox(
		static_cast<int>(m_center_pos.x - m_icon_width  * 0.5f),
		static_cast<int>(m_center_pos.y - m_icon_height * 0.5f),
		static_cast<int>(m_center_pos.x + m_icon_width  * 0.5f),
		static_cast<int>(m_center_pos.y + m_icon_height * 0.5f),
		0xffffff, FALSE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (m_graphicer)
	{
		m_graphicer->Draw();
	}
}

void WeaponShortcutIcon::DrawToMaskResource() const
{
	DrawBox(
		static_cast<int>(m_center_pos.x - m_icon_width  * 0.5f),
		static_cast<int>(m_center_pos.y - m_icon_height * 0.5f),
		static_cast<int>(m_center_pos.x + m_icon_width  * 0.5f),
		static_cast<int>(m_center_pos.y + m_icon_height * 0.5f),
		0xffffff, TRUE);
}

void WeaponShortcutIcon::AttachGraphic(const std::shared_ptr<Graphicer>& graphicer)
{
	// è„èëÇ´ïsâ¬
	if (m_graphicer) { return; }

	m_graphicer = graphicer;
	m_graphicer->SetCenterPos(m_center_pos);
}
