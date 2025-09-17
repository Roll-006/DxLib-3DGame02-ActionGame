#pragma once
#include <functional>
#include <typeindex>

#include "../Base/singleton_base.hpp"

#include "weapon_shot_data.hpp"
#include "rocket_launcher_shot_data.hpp"
#include "on_shot_bullet_data.hpp"
#include "on_hit_bullet_data.hpp"
#include "start_rocket_launcher_cutscene_data.hpp"
#include "end_rocket_launcher_cutscene_data.hpp"
#include "grab_data.hpp"
#include "grabbed_data.hpp"
#include "end_grab_cutscene_data.hpp"
#include "enter_near_death_data.hpp"
#include "on_select_normal_filter_data.hpp"
#include "on_select_cinematic_filter_data.hpp"
#include "on_select_retro_filter_data.hpp"

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
