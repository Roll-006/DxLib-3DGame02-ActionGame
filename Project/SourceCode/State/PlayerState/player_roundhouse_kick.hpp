#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
    class RoundhouseKick final : public PlayerStateBase
    {
    public:
        RoundhouseKick(Player& player, State& state, const std::shared_ptr<Animator>& animator);
        ~RoundhouseKick();

        void Update() override;
        void LateUpdate() override;
        void Enter() override;
        void Exit() override;

        [[nodiscard]] const PlayerStateKind GetNextStateKind() override;

    private:
        static constexpr float kAttackTriggerRadius = 19.0f;

        bool m_has_trigger_created;
        bool m_has_trigger_deleted;
    };
}
