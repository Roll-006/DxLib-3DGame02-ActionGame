#include "weapon_graphic_getter.hpp"

WeaponGraphicGetter::WeaponGraphicGetter()
{
	m_weapon_graphics[ObjName.ASSAULT_RIFLE] = std::make_shared<Graphicer>(UIGraphicPath.ASSAULT_RIFLE);
	m_weapon_graphics[ObjName.ASSAULT_RIFLE]->SetScale(0.07f);

	m_weapon_graphics[ObjName.ROCKET_LAUNCHER] = std::make_shared<Graphicer>(UIGraphicPath.ROCKET_LAUNCHER);
	m_weapon_graphics[ObjName.ROCKET_LAUNCHER]->SetScale(0.05f);

	m_weapon_graphics[ObjName.KNIFE] = std::make_shared<Graphicer>(UIGraphicPath.KNIFE);
	m_weapon_graphics[ObjName.KNIFE]->SetScale(0.07f);
}

WeaponGraphicGetter::~WeaponGraphicGetter()
{

}
