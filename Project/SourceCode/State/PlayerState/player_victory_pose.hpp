#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
    class VictoryPose final : public PlayerStateBase
    {
    public:
        VictoryPose(Player& player, State& state, const std::shared_ptr<Animator>& animator);
        ~VictoryPose();

        void Update() override;
        void LateUpdate() override;
        void Enter() override;
        void Exit() override;

        [[nodiscard]] const PlayerStateKind GetNextStateKind() override;

    private:
        std::shared_ptr<GameClearVirtualCamerasController> m_camera_controller;
    };
}
