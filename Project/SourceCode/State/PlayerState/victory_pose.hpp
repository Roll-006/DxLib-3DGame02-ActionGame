#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"
#include "../../VirtualCamera/game_clear_virtual_cameras_controller.hpp"

namespace player_state
{
	class VictoryPose final : public ActionStateBase<Player>
	{
	public:
		VictoryPose();
		~VictoryPose() override;

		void Update		(std::shared_ptr<Player>& obj) override;
		void LateUpdate	(std::shared_ptr<Player>& obj) override;
		void Enter		(std::shared_ptr<Player>& obj) override;
		void Exit		(std::shared_ptr<Player>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player>& obj) override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }
		
	private:
		bool m_is_stop_all_state;
		std::shared_ptr<GameClearVirtualCamerasController> m_camera_controller;
	};
}