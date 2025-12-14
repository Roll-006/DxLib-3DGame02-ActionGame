#pragma once
#include "../../Base/move_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class Idle final : public MoveStateBase
	{
	public:
		Idle(Player& player);
		~Idle() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		float m_non_move_time;
		bool  m_is_stop_all_state;
		Player& m_player;
	};
}
