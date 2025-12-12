#pragma once
#include "../Interface/i_event.hpp"
#include "../Kind/event_kind.hpp"
#include "../IncludeList/event.hpp"

template <typename EventDataT>
struct Event : public IEvent
{
	EventKind	event_kind;
	EventDataT	j_data;

	Event(const EventKind event_kind, EventDataT j_data) : 
		event_kind(event_kind), j_data(j_data){ }

	[[nodiscard]] std::type_index GetType() const override { return std::type_index(typeid(EventDataT)); }
};
