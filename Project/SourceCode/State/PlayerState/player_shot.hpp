#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
    class Shot final : public PlayerStateBase
    {
    public:
        Shot(Player& player, State& state, const std::shared_ptr<Animator>& animator);
        ~Shot();

        void Update() override;
        void LateUpdate() override;
        void Enter() override;
        void Exit() override;

        [[nodiscard]] const PlayerStateKind GetNextStateKind() override;
    };
}
