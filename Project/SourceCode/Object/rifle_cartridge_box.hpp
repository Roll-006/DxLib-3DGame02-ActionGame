#pragma once
#include "../Interface/i_item.hpp"
#include "../Kind/gun_kind.hpp"

class RifleCartridgeBox final : public IItem
{
public:
	RifleCartridgeBox(const GunKind kind);
	~RifleCartridgeBox() override;

	[[nodiscard]] ItemKind GetItemKind() const override { return m_item_kind; }

private:
	GunKind  m_gun_kind;
	ItemKind m_item_kind;
	int		 m_have_num;		// Š—L”
	int		 m_max_have_num;	// Å‘åŠ—L”
};
