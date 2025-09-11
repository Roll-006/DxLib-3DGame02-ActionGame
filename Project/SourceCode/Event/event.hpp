#pragma once
#include "../Interface/i_event.hpp"

#include "../Kind/event_kind.hpp"

#include "weapon_shot_data.hpp"
#include "rocket_launcher_shot_data.hpp"
#include "on_shot_bullet_data.hpp"
#include "on_hit_bullet_data.hpp"
#include "start_rocket_launcher_cutscene_data.hpp"
#include "end_rocket_launcher_cutscene_data.hpp"

template <typename EventDataT>
struct Event : public IEvent
{
	EventKind	event_kind;
	EventDataT	data;

	Event(const EventKind event_kind, EventDataT data) : 
		event_kind(event_kind), data(data){ }

	[[nodiscard]] std::type_index GetType() const override { return std::type_index(typeid(EventDataT)); }
};
