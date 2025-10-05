#pragma once
#include "graphicer.hpp"
#include "../Name/obj_name.hpp"
#include "../Path/ui_graphic_path.hpp"

class WeaponGraphicGetter final
{
public:
	WeaponGraphicGetter();
	~WeaponGraphicGetter();

	[[nodiscard]] std::shared_ptr<Graphicer> GetWeaponGraphicer(const std::string& weapon_name) const { return m_weapon_graphics.at(weapon_name); }

private:
	std::unordered_map<std::string, std::shared_ptr<Graphicer>> m_weapon_graphics;
};
