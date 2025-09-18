#pragma once
#include "../Event/event.hpp"

class IObserver abstract
{
public:
	virtual ~IObserver() = default;

	virtual void OnNotify(const IEvent& event) abstract;
};
