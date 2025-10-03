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

		void Update		(std::shared_ptr<Player>& obj) override;
		void LateUpdate	(std::shared_ptr<Player>& obj) override;
		void Enter		(std::shared_ptr<Player>& obj) override;
		void Exit		(std::shared_ptr<Player>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player>& obj) override;
		[[nodiscard]] bool IsAllStop() const override { return m_is_all_stop; }

	private:
		static constexpr float kShotWaitTime = 2.3f;	// ’eŠÛ‚ª”­ŽË‚³‚ê‚é‚Ü‚Å‚Ì‘Ò‚¿ŽžŠÔ

		std::shared_ptr<RocketLauncherVirtualCameraController> m_rocket_launcher_camera_controller;
		float m_wait_timer;
		bool  m_was_shot;
		bool  m_is_all_stop;
	};
}
