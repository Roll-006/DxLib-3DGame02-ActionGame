#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
    class ShotRocketLauncher final : public PlayerStateBase
    {
    public:
        ShotRocketLauncher(Player& player, State& state, const std::shared_ptr<Animator>& animator);
        ~ShotRocketLauncher();

        void Update()       override;
        void LateUpdate()   override;
        void Enter()        override;
        void Exit()         override;

        [[nodiscard]] const PlayerStateKind GetNextStateKind() override;

    private:
        static constexpr float kShotWaitTime = 1.2f; // 弾丸が発射されるまでの待ち時間

        std::shared_ptr<RocketLauncherVirtualCamerasController> m_rocket_launcher_camera_controller;
        float m_wait_timer;
        bool  m_was_shot;
    };
}
