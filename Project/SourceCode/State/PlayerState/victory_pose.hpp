#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"
#include "../../VirtualCamera/game_clear_virtual_cameras_controller.hpp"

namespace player_state
{
	class VictoryPose final : public ActionStateBase
	{
	public:
		VictoryPose(Player& player);
		~VictoryPose() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }
		
	private:
		bool m_is_stop_all_state;
		std::shared_ptr<GameClearVirtualCamerasController> m_camera_controller;
		Player& m_player;
	};
}