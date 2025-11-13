#include "ammo_holder.hpp"

AmmoHolder::AmmoHolder()
{

}

AmmoHolder::~AmmoHolder()
{

}

void AmmoHolder::AddAmmo(std::shared_ptr<IAmmoBox>& ammo_box)
{
	if (!ammo_box) { return; }

	std::stack<std::shared_ptr<IAmmoBox>>* tmp_ammo_box = nullptr;
	
	if (	 typeid(*ammo_box) == typeid(AssaultRifleAmmoBox))	{ tmp_ammo_box = &m_assault_rifle_ammo_boxs; }
	else if (typeid(*ammo_box) == typeid(RocketBombBox))		{ tmp_ammo_box = &m_rocket_bomb_boxs; }
	else { return; }

	if (tmp_ammo_box->empty())
	{
		tmp_ammo_box->emplace(ammo_box);
	}
	else
	{
		// ‡¬Œã‚à’e‚ªŽc‚Á‚Ä‚¢‚ê‚Î’Ç‰Á‚·‚é
		auto& ammo = tmp_ammo_box->top();
		ammo->Synthesize(ammo_box);
		if (ammo_box->GetCurrentHaveNum() > 0)
		{
			tmp_ammo_box->emplace(ammo_box);
		}
		else
		{
			ammo_box = nullptr;
		}
	}
}
