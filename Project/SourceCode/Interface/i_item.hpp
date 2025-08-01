#pragma once
#include "../Data/Kind/item_kind.hpp"

class IItem abstract
{
public:
	virtual ~IItem() = default;

	[[nodiscard]] virtual ItemKind GetItemKind() const abstract;
};
