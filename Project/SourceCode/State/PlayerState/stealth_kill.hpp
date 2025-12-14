#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"
#include "../../VirtualCamera/stealth_kill_virtual_camera_controller.hpp"

namespace player_state
{
	class StealthKill final : public ActionStateBase
	{
	public:
		StealthKill(Player& player);
		~StealthKill() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		bool m_is_stop_all_state;
		bool m_is_stab;
		bool m_is_draw;

		std::shared_ptr<StealthKillVirtualCameraController>	m_stealth_kill_camera_controller;
		Player& m_player;
	};
}
