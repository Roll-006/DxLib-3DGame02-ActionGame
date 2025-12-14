#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class Dead final : public ActionStateBase
	{
	public:
		Dead(Player& player);
		~Dead() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		bool	m_is_stop_all_state;

		float	m_elapsed_time;
		bool	m_is_seted_time_scale;
		std::shared_ptr<DeadVirtualCamerasController>	m_dead_cameras_controller;
		Player& m_player;
	};
}
