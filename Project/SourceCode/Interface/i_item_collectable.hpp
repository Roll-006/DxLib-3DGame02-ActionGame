#pragma once
#include <memory>
#include "i_item.hpp"
#include "../Event/spotted_item_event.hpp"
#include "../Data/spotted_obj_data.hpp"

class IItemCollectable abstract
{
public:
	virtual ~IItemCollectable() = default;

	virtual void StopAddPickupableItem() abstract;

	virtual void PickUpItem() abstract;

	virtual void AddItem	(const std::shared_ptr<IItem>& item) abstract;
	virtual void RemoveItem	(const std::shared_ptr<IItem>& item) abstract;

	virtual void AddPickUpCandidateItem(const SpottedItemEvent& event) abstract;
	virtual void RemovePickUpCandidateItem(const int obj_handle) abstract;
	virtual void RemovePickUpCandidateItems() abstract;

	virtual void AddPickupableItem(const std::shared_ptr<IItem>& item) abstract;
	virtual void RemovePickupableItem() abstract;

	[[nodiscard]] virtual std::shared_ptr<IItem>&				GetPickupableItem()		abstract;
	[[nodiscard]] virtual const std::vector<SpottedObjData>&	GetCandidateItems()		abstract;
	[[nodiscard]] virtual const bool							CanAddPickupableItem()	abstract;
};
