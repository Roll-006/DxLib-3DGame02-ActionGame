#pragma once
#include "../Kind/item_kind.hpp"
#include "../Part/transform.hpp"

class IItem abstract
{
public:
	virtual ~IItem() = default;

	[[nodiscard]] virtual std::shared_ptr<Transform> GetItemTransform() const abstract;
	[[nodiscard]] virtual ItemKind GetItemKind() const abstract;
};
