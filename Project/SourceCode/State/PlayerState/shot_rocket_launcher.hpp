#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"
namespace player_state
{
	class ShotRocketLauncher final : public WeaponActionStateBase<Player>
	{
	public:
		ShotRocketLauncher();
		~ShotRocketLauncher() override;

		void Update		(Player* obj) override;
		void LateUpdate	(Player* obj) override;
		void Enter		(Player* obj) override;
		void Exit		(Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(Player* obj) override;

	private:
		static constexpr float kShotWaitTime = 5.0f;	// ’eŠÛ‚ª”­ŽË‚³‚ê‚é‚Ü‚Å‚Ì‘Ò‚¿ŽžŠÔ

		std::shared_ptr<RocketLauncherVirtualCameraController> m_rocket_launcher_camera_controller;
		float m_wait_timer;
		bool  m_was_shot;

		float test;
	};
}
