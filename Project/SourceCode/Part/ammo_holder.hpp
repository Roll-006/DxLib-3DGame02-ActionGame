#pragma once
#include <typeindex>
#include <stack>

#include "../Object/assault_rifle_ammo_box.hpp"
#include "../Object/rocket_bomb_box.hpp"

class AmmoHolder final
{
public:
	AmmoHolder();
	~AmmoHolder();

	/// @brief ’e‚ğ’Ç‰Á‚·‚é
	/// @param ammo_box ’Ç‰Á‚·‚é’e(‡¬‚É‚æ‚Á‚ÄÁ‚¦‚½ê‡Anullptr‚Æ‚È‚é)
	[[nodiscard]] void AddAmmo(std::shared_ptr<IAmmoBox>& ammo_box);

private:
	std::stack<std::shared_ptr<IAmmoBox>> m_assault_rifle_ammo_boxs;
	std::stack<std::shared_ptr<IAmmoBox>> m_rocket_bomb_boxs;
};
