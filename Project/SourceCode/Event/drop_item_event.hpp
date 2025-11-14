#pragma once
#include "../Part/transform.hpp"
#include "../Kind/item_kind.hpp"

/// @brief アイテムをドロップした
struct DropItemEvent
{
	std::shared_ptr<Transform>	drop_transform	= nullptr;
	int							obj_handle		= -1;
	ItemKind					item_kind		= ItemKind::kNone;
};
