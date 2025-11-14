#pragma once
#include <typeindex>
#include <stack>

#include "../Object/assault_rifle_ammo_box.hpp"
#include "../Object/rocket_bomb_box.hpp"
#include "../Base/gun_base.hpp"

class AmmoHolder final
{
public:
	AmmoHolder();
	~AmmoHolder();

	/// @brief ’e‚ğ’Ç‰Á‚·‚é
	/// @param ammo_box ’Ç‰Á‚·‚é’e(‡¬‚É‚æ‚Á‚ÄÁ‚¦‚½ê‡Anullptr‚Æ‚È‚é)
	void AddAmmo(std::shared_ptr<IAmmoBox>& ammo_box);

	void Reload(const std::shared_ptr<GunBase>& gun);

	[[nodiscard]] int GetCurrentAmmoNum(const std::type_index& type);

private:
	std::stack<std::shared_ptr<IAmmoBox>> m_assault_rifle_ammo_boxes;
	std::stack<std::shared_ptr<IAmmoBox>> m_rocket_bomb_boxes;
};
