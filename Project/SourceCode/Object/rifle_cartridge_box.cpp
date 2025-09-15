#include "rifle_cartridge_box.hpp"

RifleCartridgeBox::RifleCartridgeBox(const GunKind kind) :
	m_gun_kind		(kind),
	m_item_kind		(ItemKind::kRifleCartridge),
	m_have_num		(0),
	m_max_have_num	(0)
{

}

RifleCartridgeBox::~RifleCartridgeBox()
{

}
