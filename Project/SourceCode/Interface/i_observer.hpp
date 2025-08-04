#pragma once
#include "../Base/obj_base.hpp"
#include "../Data/Kind/event_kind.hpp"

class IObserver abstract
{
public:
	virtual ~IObserver() = default;

	virtual void OnNotify(const std::shared_ptr<ObjBase> obj, const EventKind event_kind) abstract;
};