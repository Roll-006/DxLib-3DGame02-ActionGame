#pragma once
#include <memory>
#include "i_item.hpp"

class IItemCollectable abstract
{
public:
	virtual ~IItemCollectable() = default;

	virtual void AddItem	(const std::shared_ptr<IItem>& item) abstract;
	virtual void RemoveItem	(const std::shared_ptr<IItem>& item) abstract;
};
