#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
    class SpinningSlashKnife final : public PlayerStateBase
    {
    public:
        SpinningSlashKnife(Player& player, State& state, const std::shared_ptr<Animator>& animator);
        ~SpinningSlashKnife();

        void Update() override;
        void LateUpdate() override;
        void Enter() override;
        void Exit() override;

        [[nodiscard]] const PlayerStateKind GetNextStateKind() override;

    private:
        static constexpr float kComboValidTime = 1.0f; // 二段階目の攻撃が有効になる時間

        float m_combo_timer;
        bool  m_has_trigger_created;
        bool  m_has_trigger_deleted;
    };
}
