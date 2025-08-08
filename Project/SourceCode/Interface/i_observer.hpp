#pragma once
#include "../Data/Event/event.hpp"

class IObserver abstract
{
public:
	virtual ~IObserver() = default;

	virtual void OnNotify(const IEvent& event) abstract;
};