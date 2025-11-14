#include "rocket_bomb_box.hpp"

RocketBombBox::RocketBombBox() : 
	rifle_cartridge_box_data(),
	m_have_num				(0)
{
	JSONLoader json_loader;
	nlohmann::json data;
	if (json_loader.Load("Data/JSON/ammo_box.json", data))
	{
		rifle_cartridge_box_data = data.at("ammo_box").at("assault_rifle_ammo_box").at("rocket_bomb_box").get<RifleCartridgeBoxData>();
	}
}

RocketBombBox::RocketBombBox(const int ammo_num) :
	rifle_cartridge_box_data(),
	m_have_num(ammo_num)
{
	JSONLoader json_loader;
	nlohmann::json data;
	if (json_loader.Load("Data/JSON/ammo_box.json", data))
	{
		rifle_cartridge_box_data = data.at("ammo_box").at("assault_rifle_ammo_box").at("rocket_bomb_box").get<RifleCartridgeBoxData>();
	}

	if (m_have_num > GetMaxHaveNum())
	{
		m_have_num = GetMaxHaveNum();
	}
}

RocketBombBox::~RocketBombBox()
{

}

void RocketBombBox::Synthesize(const std::shared_ptr<IAmmoBox> ammo_box)
{
	if (IsMax()) { return; }

	const auto partner_num	= ammo_box->GetCurrentHaveNum();	// Œ»Ý‚Ì‘ŠŽè‚Ì”
	const auto margin_num	= GetMaxHaveNum() - m_have_num;		// ‹ó‚«—e—Ê
	const auto sub_num		= partner_num - margin_num;			// ‚ ‚Ó‚ê‚½”

	AddHaveNum(partner_num);

	if (sub_num > 0)
	{
		ammo_box->SetHaveNum(sub_num);
	}
}

void RocketBombBox::SetHaveNum(const int set_num)
{
	m_have_num = set_num;
	if (m_have_num > GetMaxHaveNum())
	{
		m_have_num = GetMaxHaveNum();
	}
}

void RocketBombBox::AddHaveNum(const int add_num)
{
	m_have_num += add_num;
	if (m_have_num > GetMaxHaveNum())
	{
		m_have_num = GetMaxHaveNum();
	}
}
