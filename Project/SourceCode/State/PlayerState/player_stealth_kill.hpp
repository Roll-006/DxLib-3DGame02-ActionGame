#pragma once
#include "../../Base/player_state_base.hpp"

class StealthKillVirtualCameraController;
class Animator;

namespace player_state
{
    class StealthKill final : public PlayerStateBase
    {
    public:
        StealthKill(Player& player, State& state, const std::shared_ptr<Animator>& animator);
        ~StealthKill();

        void Update() override;
        void LateUpdate() override;
        void Enter() override;
        void Exit() override;

        [[nodiscard]] const PlayerStateKind GetNextStateKind() override;

    private:
        bool m_is_stab;
        bool m_is_draw;

        std::shared_ptr<StealthKillVirtualCameraController> m_stealth_kill_camera_controller;
    };
}
