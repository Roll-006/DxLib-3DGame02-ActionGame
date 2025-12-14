#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class ShotRocketLauncher final : public WeaponActionStateBase
	{
	public:
		ShotRocketLauncher(Player& player);
		~ShotRocketLauncher() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		static constexpr float kShotWaitTime = 1.2f;	// 弾丸が発射されるまでの待ち時間

		std::shared_ptr<RocketLauncherVirtualCamerasController> m_rocket_launcher_camera_controller;
		float m_wait_timer;
		bool  m_was_shot;
		bool  m_is_stop_all_state;
		Player& m_player;
	};
}
