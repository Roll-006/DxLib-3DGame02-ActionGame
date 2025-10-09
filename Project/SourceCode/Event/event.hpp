#pragma once
#include "../Interface/i_event.hpp"

#include "../Kind/event_kind.hpp"

#include "weapon_shot_event.hpp"
#include "rocket_launcher_shot_event.hpp"
#include "on_shot_bullet_event.hpp"
#include "on_hit_bullet_event.hpp"
#include "on_damage_event.hpp"
#include "start_rocket_launcher_cutscene_event.hpp"
#include "end_rocket_launcher_cutscene_event.hpp"
#include "grab_event.hpp"
#include "grabbed_event.hpp"
#include "release_event.hpp"
#include "end_grab_cutscene_event.hpp"
#include "enter_near_death_event.hpp"
#include "near_death_event.hpp"
#include "on_select_normal_filter_event.hpp"
#include "on_select_cinematic_filter_event.hpp"
#include "on_select_retro_filter_event.hpp"
#include "on_downed_enemy_spotted_event.hpp"
#include "on_melee_target_lost_event.hpp"

template <typename EventDataT>
struct Event : public IEvent
{
	EventKind	event_kind;
	EventDataT	data;

	Event(const EventKind event_kind, EventDataT data) : 
		event_kind(event_kind), data(data){ }

	[[nodiscard]] std::type_index GetType() const override { return std::type_index(typeid(EventDataT)); }
};
