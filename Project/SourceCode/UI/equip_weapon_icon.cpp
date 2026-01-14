#include "equip_weapon_icon.hpp"

EquipWeaponIcon::EquipWeaponIcon(const std::unordered_map<WeaponSlotKind, std::shared_ptr<WeaponBase>>& equip_weapons) : 
	m_equip_weapons			(equip_weapons),
	m_weapon_graphic		(std::make_shared<WeaponGraphicGetter>()),
	m_main_weapon_graphic	(nullptr),
	m_sub_weapon_graphic	(nullptr),
	m_result_screen			(std::make_shared<ScreenCreator>(kScreenSize))
{
	SetWeaponGraphic(WeaponSlotKind::kMain, m_main_weapon_graphic);
	SetWeaponGraphic(WeaponSlotKind::kSub,	m_sub_weapon_graphic);

	m_main_weapon_graphic->SetCenterPos(kCenterPos);
	m_sub_weapon_graphic ->SetCenterPos(kSubWeaponDrawPos);
	m_sub_weapon_graphic ->SetScale(0.08f);
}

EquipWeaponIcon::~EquipWeaponIcon()
{

}

void EquipWeaponIcon::LateUpdate()
{
	SetWeaponGraphic(WeaponSlotKind::kMain, m_main_weapon_graphic);
	SetWeaponGraphic(WeaponSlotKind::kSub,	m_sub_weapon_graphic);

	CreateResultScreen();
}

void EquipWeaponIcon::CreateResultScreen()
{
	m_result_screen->UseScreen();

	if (m_main_weapon_graphic)	{ m_main_weapon_graphic	->Draw(); }
	if (m_sub_weapon_graphic)	{ m_sub_weapon_graphic	->Draw(); }

	m_result_screen->UnuseScreen();
}

void EquipWeaponIcon::SetWeaponGraphic(const WeaponSlotKind equip_weapon_slot, std::shared_ptr<Graphicer>& weapon_graphic)
{
	const auto equip_weapon = m_equip_weapons.at(equip_weapon_slot);

	if (!weapon_graphic && !equip_weapon) { return; }
	if ( weapon_graphic && !equip_weapon) { weapon_graphic = nullptr; return; }

	const auto graphic = m_weapon_graphic->GetWeaponGraphicer(equip_weapon->GetName());

	if (weapon_graphic != graphic)
	{
		weapon_graphic = graphic;

		switch (equip_weapon_slot)
		{
		case WeaponSlotKind::kMain:
			weapon_graphic->SetCenterPos(kCenterPos);
			break;

		case WeaponSlotKind::kSub:
			weapon_graphic->SetCenterPos(kSubWeaponDrawPos);
			weapon_graphic->SetScale	(0.08f);
			break;
		}
	}
}
