#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
	class Dead final : public PlayerStateBase
	{
	public:
		Dead(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator);
		~Dead();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const PlayerStateKind GetNextStateKind() override;

	private:
		bool m_is_stop_all_state;
		float m_elapsed_time;
		bool m_is_seted_time_scale;
		std::shared_ptr<DeadVirtualCamerasController> m_dead_cameras_controller;
	};
}

