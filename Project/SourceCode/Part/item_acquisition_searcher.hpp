#pragma once
#include "../Interface/i_item_collectable.hpp"
#include "../Calculation/math.hpp"
#include "../Event/event_system.hpp"

#include "../Accessor/obj_accessor.hpp"

class ItemAcquisitionSearcher final
{
public:
	ItemAcquisitionSearcher(const std::weak_ptr<IItemCollectable>& item_collectable);
	~ItemAcquisitionSearcher();

	void Update();

private:
	void SearchItemAcquisition();

private:
	static constexpr float kPrioritySwitchAngle = 30.0f;

	std::weak_ptr<IItemCollectable> m_item_collectable;
};
