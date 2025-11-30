#pragma once
#include <memory>
#include "i_item.hpp"
#include "../Event/spotted_item_event.hpp"
#include "../Data/spotted_obj_data.hpp"

class IItemCollectable abstract
{
public:
	virtual ~IItemCollectable() = default;

	virtual void StopAddAcquirableItem() abstract;

	virtual void AddItem	(const std::shared_ptr<IItem>& item) abstract;
	virtual void RemoveItem	(const std::shared_ptr<IItem>& item) abstract;

	virtual void AddGetCandidateItem(const SpottedItemEvent& event) abstract;
	virtual void RemoveGetCandidateItems() abstract;

	virtual void AddAcquirableItem(const std::shared_ptr<IItem>& item) abstract;
	virtual void RemoveAcquirableItem() abstract;

	[[nodiscard]] virtual std::shared_ptr<IItem>&		GetAcquirableItem()		abstract;
	[[nodiscard]] virtual std::vector<SpottedObjData>&	GetCandidateItems()		abstract;
	[[nodiscard]] virtual bool							CanAddAcquirableItem()	abstract;
};
