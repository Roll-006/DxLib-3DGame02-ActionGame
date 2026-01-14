#pragma once
#include "../Base/weapon_base.hpp"
#include "../Interface/i_weapon_equippable.hpp"
#include "../Part/weapon_graphic_getter.hpp"
#include "../Part/screen_creator.hpp"

/// @brief 装備中の武器のUI表示を行う
/// MEMO : HPゲージと合成するためStatusIconsクラスにて描画される
class EquipWeaponIcon final
{
public:
	EquipWeaponIcon(const std::unordered_map<WeaponSlotKind, std::shared_ptr<WeaponBase>>& equip_weapons);
	~EquipWeaponIcon();

	void LateUpdate();

	[[nodiscard]] int GetScreenHandle() const { return m_result_screen->GetScreenHandle(); }

private:
	void CreateResultScreen();
	void SetWeaponGraphic(const WeaponSlotKind equip_weapon_slot, std::shared_ptr<Graphicer>& weapon_graphic);

private:
	static constexpr Vector2D<int>	kScreenSize			= { 256, 256 };
	static constexpr Vector2D<int>	kCenterPos			= { static_cast<int>(kScreenSize.x * 0.5f), static_cast<int>(kScreenSize.y * 0.5f) };
	static constexpr Vector2D<int>	kSubWeaponDrawPos	= { static_cast<int>(kScreenSize.x * 0.3f), static_cast<int>(kScreenSize.y * 0.2f) };

	const std::unordered_map<WeaponSlotKind, std::shared_ptr<WeaponBase>>& m_equip_weapons;
	std::shared_ptr<WeaponGraphicGetter>	m_weapon_graphic;
	std::shared_ptr<Graphicer>				m_main_weapon_graphic;
	std::shared_ptr<Graphicer>				m_sub_weapon_graphic;
	std::shared_ptr<ScreenCreator>			m_result_screen;	// 最終的な描画結果を描画するスクリーン
};
