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

	std::stack<std::shared_ptr<IAmmoBox>>* ammo_boxes = nullptr;
	
	if (	 typeid(*ammo_box) == typeid(AssaultRifleAmmoBox))	{ ammo_boxes = &m_assault_rifle_ammo_boxes; }
	else if (typeid(*ammo_box) == typeid(RocketBombBox))		{ ammo_boxes = &m_rocket_bomb_boxes; }
	else { return; }

	if (ammo_boxes->empty())
	{
		ammo_boxes->emplace(ammo_box);
	}
	else
	{
		// ‡¬Œã‚à’e‚ªŽc‚Á‚Ä‚¢‚ê‚Î’Ç‰Á‚·‚é
		auto& tmp_ammo_box = ammo_boxes->top();
		tmp_ammo_box->Synthesize(ammo_box);
		if (ammo_box->GetCurrentHaveNum() > 0)
		{
			ammo_boxes->emplace(ammo_box);
		}
		else
		{
			ammo_box = nullptr;
		}
	}
}

void AmmoHolder::Reload(const std::shared_ptr<GunBase>& gun)
{
	std::stack<std::shared_ptr<IAmmoBox>>* ammo_boxes = nullptr;
	const auto gun_kind		= gun->GetGunKind();
	auto	   current_ammo = 0;

	switch (gun_kind)
	{
	case GunKind::kSubmachineGun:
		current_ammo	= GetCurrentAmmoNum(typeid(AssaultRifleAmmoBox));
		ammo_boxes		= &m_assault_rifle_ammo_boxes;
		break;

	case GunKind::kRocketLauncher:
		current_ammo	= GetCurrentAmmoNum(typeid(RocketBombBox));
		ammo_boxes		= &m_rocket_bomb_boxes;
		break;

	default:
		break;
	}

	if (!ammo_boxes) { return; }

	// ‘•“U‚µ‚½•ª‚ðŒ¸‚ç‚·
	const auto rest_ammo	= gun->OnReload(current_ammo);
	auto	   added_ammo	= current_ammo - rest_ammo;
	while (!ammo_boxes->empty())
	{
		const auto tmp_ammo_box			= ammo_boxes->top();
		const auto current_have_ammo	= tmp_ammo_box->GetCurrentHaveNum();

		// ‘•“U‚µ‚½•ª‚ðtop‚©‚çŒ¸‚ç‚µ‚Ä‚¢‚­
		// ’e‚ªŽc‚ç‚È‚¢ê‡‚ÍŽæ‚èœ‚­
		added_ammo -= current_have_ammo;
		if (added_ammo >= 0)
		{
			ammo_boxes->pop();
		}
		else
		{
			tmp_ammo_box->SetHaveNum(-added_ammo);
			break;
		}
	}
}

int AmmoHolder::GetCurrentAmmoNum(const std::type_index& type)
{
	std::stack<std::shared_ptr<IAmmoBox>>* ammo_boxes = nullptr;

	if (	 type == typeid(AssaultRifleAmmoBox))	{ ammo_boxes = &m_assault_rifle_ammo_boxes; }
	else if (type == typeid(RocketBombBox))			{ ammo_boxes = &m_rocket_bomb_boxes; }
	else { return 0; }

	int ammo_sum = 0;
	auto tmp = *ammo_boxes;
	while (!tmp.empty())
	{
		const auto tmp_ammo_box = tmp.top();
		tmp.pop();

		ammo_sum += tmp_ammo_box->GetCurrentHaveNum();
	}
	return ammo_sum;
}
