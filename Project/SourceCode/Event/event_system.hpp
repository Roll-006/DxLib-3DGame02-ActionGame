#pragma once
#include <functional>
#include <typeindex>

#include "../Base/singleton_base.hpp"

#include "weapon_shot_event.hpp"
#include "rocket_launcher_shot_event.hpp"
#include "on_shot_bullet_event.hpp"
#include "on_hit_bullet_event.hpp"
#include "on_damage_event.hpp"
#include "start_rocket_launcher_cutscene_event.hpp"
#include "end_rocket_launcher_cutscene_event.hpp"
#include "grab_event.hpp"
#include "grabbed_event.hpp"
#include "end_grab_cutscene_event.hpp"
#include "enter_near_death_event.hpp"
#include "near_death_event.hpp"
#include "on_select_normal_filter_event.hpp"
#include "on_select_cinematic_filter_event.hpp"
#include "on_select_retro_filter_event.hpp"

class EventSystem final : public SingletonBase<EventSystem>
{
public:
    // イベントリスナーを登録
    template<typename EventT>
    void Subscribe(std::function<void(const EventT&)> callback)
    {
        const auto type_id = std::type_index(typeid(EventT));

        // コールバックをラップして保存
        m_listeners[type_id].emplace_back([callback](const void* event_data)
        {
            const auto* event = static_cast<const EventT*>(event_data);
            callback(*event);
        });
    }

    // イベントを発行
    template<typename EventT>
    void Publish(const EventT& event)
    {
        const auto type_id = std::type_index(typeid(EventT));

        auto itr = m_listeners.find(type_id);
        if (itr != m_listeners.end())
        {
            for (auto& callback : itr->second)
            {
                callback(&event);
            }
        }
    }

private:
    EventSystem() {}
    ~EventSystem() {}

private:
    std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> m_listeners;

    friend SingletonBase<EventSystem>;
};
